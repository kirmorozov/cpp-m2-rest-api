
/*
   Mathieu Stefani, 07 février 2016
   Example of a REST endpoint with routing
*/

#include <algorithm>

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>

#include <mysqlx/xdevapi.h>

#include "encryptor.h"

//#include "json.hpp"
#include "Error-response.h"
#include "Error-response.cpp"
#include "V1IntegrationAdminTokenPostBody.h"
#include "V1IntegrationAdminTokenPostBody.cpp"
#include "store/V1StoreView.h"
#include "store/V1StoreView.cpp"
#include "store/V1StoreGroup.h"
#include "store/V1StoreGroup.cpp"

#include <thread>
#include <iostream>
#include <filesystem>

using namespace std;
using namespace mysqlx;
using namespace Pistache;

using m2_encryptor = mg::m2::Encryptor;

using namespace io::swagger::server::model;


using sharedClient = std::shared_ptr<Client>;
using sharedEncryptor = std::shared_ptr<m2_encryptor>;
namespace fs = std::filesystem;


void printCookies(const Http::Request &req) {
    auto cookies = req.cookies();
    std::cout << "Cookies: [" << std::endl;
    const std::string indent(4, ' ');
    for (const auto &c: cookies) {
        std::cout << indent << c.name << " = " << c.value << std::endl;
    }
    std::cout << "]" << std::endl;
}

namespace Generic {

    void handleReady(const Rest::Request &, Http::ResponseWriter response) {
        auto thr_id = std::this_thread::get_id();
        std::stringstream respBuffer;
        respBuffer << "Current Thread: " << thr_id;
        response.send(Http::Code::Ok, respBuffer.str());
    }

}

class MG_M2_API_point {
public:
    MG_M2_API_point(Address addr)
            : httpEndpoint(std::make_shared<Http::Endpoint>(addr)) {}

    void init(sharedClient connection, sharedEncryptor enc, size_t thr = 2) {
        auto opts = Http::Endpoint::options()
                .threads(thr);
        httpEndpoint->init(opts);
        setupRoutes();
        dbConnection = connection;
        encryptor = enc;
    }

    void start() {
        httpEndpoint->setHandler(router.handler());
        httpEndpoint->serve();
        cout << "Running" <<endl;
    }

protected:
    void setupRoutes() {
        using namespace Rest;
        std::string base = "/index.php/rest/V1";
//        std::string base = "/V1";
        Routes::Post(router, base + "/integration/admin/token",
                     Routes::bind(&MG_M2_API_point::V1_integration_admin_token_post_handler, this));
        Routes::Get(router, base + "/store/storeViews",
                    Routes::bind(&MG_M2_API_point::V1_store_storeViews_get_handler, this));
        Routes::Get(router, base + "/store/storeGroups",
                    Routes::bind(&MG_M2_API_point::V1_store_storeGroups_get_handler, this));
        Routes::Get(router, base + "/modules",
                    Routes::bind(&MG_M2_API_point::V1_modules_get_handler, this));

        Routes::Get(router, "/_ready", Routes::bind(&Generic::handleReady));
    }

    void V1_integration_admin_token_post_handler(const Rest::Request &request, Http::ResponseWriter response) {

        V1IntegrationAdminTokenPostBody PostBody;

        try {
            nlohmann::json request_body = nlohmann::json::parse(request.body());
            PostBody.fromJson(request_body);
            this->V1_integration_admin_token_post(PostBody, response);
        } catch (std::runtime_error &e) {
            //send a 400 error
            Error_response error;
            error.setMessage(e.what());
            response.send(Pistache::Http::Code::Bad_Request, error.toJson().dump());
            return;
        } catch (exception &e) {
            Error_response error;
            error.setMessage(e.what());
            response.send(Pistache::Http::Code::Bad_Request, error.toJson().dump());
            return;
        }
    }

    void V1_integration_admin_token_post(const V1IntegrationAdminTokenPostBody &body,
                                         Pistache::Http::ResponseWriter &response) {

        bool validAdmin = false;
        int adminId = 0;

        auto sess = dbConnection->getSession();
        {
            auto db = sess.getDefaultSchema();
            auto admin_table = db.getTable("admin_user");
            //  select password from admin_user where username = ? limit 1;
            auto res = admin_table.select("user_id", "password")
                    .where("username = :user")
                    .limit(1)
                    .bind("user", body.getUsername())
                    .execute();
            Row data = res.fetchOne();
            auto passwd = data[0];
            if (!data[0].isNull()) {
                // +70-75 ms extra for request.
                validAdmin = encryptor->validateHash(body.getPassword(), std::string(data[1]));
            }
            if (validAdmin) {
                adminId = int(data[0]);
            }
        }
        if (validAdmin) {
            nlohmann::json result = _createAdminToken(adminId);
            response.send(Pistache::Http::Code::Ok, result.dump());
        } else {
            Error_response error;
            error.setMessage("Access forbidden");
            error.setCode(int(Pistache::Http::Code::Forbidden));
            response.send(Pistache::Http::Code::Forbidden, error.toJson().dump());
        }
    }

    void admin_acl_check(const Rest::Request &request, std::string resource) {

        Error_response error(Pistache::Http::Code::Unauthorized,
                             std::string("The consumer isn't authorized to access %resources."));
        nlohmann::json paramsJson;
        paramsJson["resources"] = resource;
        error.setParameters(paramsJson);

        bool authorized = false;
        auto sess = dbConnection->getSession();
        {
            if (!request.headers().has("Authorization")) {
                throw error;
            }
            auto authheader = request.headers().get<Pistache::Http::Header::Authorization>();
            auto auth_str = authheader->value();
            std::string token = auth_str.substr(7, 32);

            auto result = sess.sql("select at.admin_id from oauth_token as at\n"
                                   "inner join authorization_role ar ON ar.user_id = at.admin_id\n"
                                   "inner join authorization_rule a on ar.parent_id = a.role_id\n"
                                   "where at.token = ?\n"
                                   "    and a.resource_id in ('Magento_Backend::all', ?)"
                                   "    and a.permission='allow'")
                    .bind(token)
                    .bind(resource)
                    .execute();
            auto rowList = result.fetchAll();
            if (result.count() > 0) {
                authorized = true;
            }
        }

        if (!authorized) {
            throw error;
        }
    }

    void V1_store_storeViews_get_handler(const Rest::Request &request, Http::ResponseWriter response) {

//        V1IntegrationAdminTokenPostBody PostBody;
        std::string _acl_resource = "Magento_Backend::store";
        try {
            this->admin_acl_check(request, _acl_resource);
            // nlohmann::json request_body = nlohmann::json::parse(request.body());
            // PostBody.fromJson(request_body);
            this->V1_store_storeViews_get(request, response);
        } catch (std::runtime_error &e) {
            //send a 400 error
            Error_response error;
            error.setMessage(e.what());
            response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
            response.send(Pistache::Http::Code::Bad_Request, error.toJson().dump());
            return;
        } catch (exception &e) {
            Error_response error;
            error.setMessage(e.what());
            response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
            response.send(Pistache::Http::Code::Bad_Request, error.toJson().dump());
            return;
        } catch (Error_response &e) {
            response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
            response.send(e.getHttpCode(), e.toJson().dump());
        }
    }

    void V1_store_storeViews_get(const Rest::Request &request, Pistache::Http::ResponseWriter &response) {

        bool validAdmin = false;
        int adminId = 0;

        std::list<nlohmann::json> result;

        auto sess = dbConnection->getSession();
        {
            auto db = sess.getDefaultSchema();
            auto store_table = db.getTable("store");
            //  select password from admin_user where username = ? limit 1;
            auto res = store_table.select().execute();
            auto data = res.fetchAll();
            const Columns &columns = res.getColumns();
            for (Row row : data) {
                V1StoreView store;
                for (unsigned index = 0; index < res.getColumnCount(); ++index) {
                    if (columns[index].getColumnName() == "store_id") {
                        store.setId(row[index]);
                    }
                    if (columns[index].getColumnName() == "code") {
                        store.setCode(std::string(row[index]));
                    }
                    if (columns[index].getColumnName() == "name") {
                        store.setName(std::string(row[index]));
                    }
                    if (columns[index].getColumnName() == "website_id") {
                        store.setWebsiteId(row[index]);
                    }
                    if (columns[index].getColumnName() == "group_id") {
                        store.setStoreGroupId(row[index]);
                    }
                    if (columns[index].getColumnName() == "is_active") {
                        store.setIsActive(row[index]);
                    }
                }
                result.push_back(store.toJson());

            }
        }

        nlohmann::json json_result(result);
        response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
        response.send(Pistache::Http::Code::Bad_Request, json_result.dump());

    }


    void V1_store_storeGroups_get_handler(const Rest::Request &request, Http::ResponseWriter response) {

//        V1IntegrationAdminTokenPostBody PostBody;
        std::string _acl_resource = "Magento_Backend::store";
        try {
            this->admin_acl_check(request, _acl_resource);
            // nlohmann::json request_body = nlohmann::json::parse(request.body());
            // PostBody.fromJson(request_body);
            this->V1_store_storeGroups_get(request, response);
        } catch (std::runtime_error &e) {
            //send a 400 error
            Error_response error;
            error.setMessage(e.what());
            response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
            response.send(Pistache::Http::Code::Bad_Request, error.toJson().dump());
            return;
        } catch (exception &e) {
            Error_response error;
            error.setMessage(e.what());
            response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
            response.send(Pistache::Http::Code::Bad_Request, error.toJson().dump());
            return;
        } catch (Error_response &e) {
            response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
            response.send(e.getHttpCode(), e.toJson().dump());
        }
    }


    void V1_store_storeGroups_get(const Rest::Request &request, Pistache::Http::ResponseWriter &response) {

        bool validAdmin = false;
        int adminId = 0;

        std::list<nlohmann::json> result;

        auto sess = dbConnection->getSession();
        {
            auto db = sess.getDefaultSchema();
            auto store_table = db.getTable("store_group");
            //  select password from admin_user where username = ? limit 1;
            auto res = store_table.select().execute();
            auto data = res.fetchAll();
            const Columns &columns = res.getColumns();
            for (Row row : data) {
                V1StoreGroup store;
                for (unsigned index = 0; index < res.getColumnCount(); ++index) {
                    if (columns[index].getColumnName() == "group_id") {
                        store.setId(row[index]);
                    }
                    if (columns[index].getColumnName() == "code") {
                        store.setCode(std::string(row[index]));
                    }
                    if (columns[index].getColumnName() == "name") {
                        store.setName(std::string(row[index]));
                    }
                    if (columns[index].getColumnName() == "website_id") {
                        store.setWebsiteId(row[index]);
                    }
                    if (columns[index].getColumnName() == "root_category_id") {
                        store.setRootCategoryId(row[index]);
                    }
                    if (columns[index].getColumnName() == "default_store_id") {
                        store.setDefaultStoreId(row[index]);
                    }
                }
                result.push_back(store.toJson());

            }
        }

        nlohmann::json json_result(result);
        response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
        response.send(Pistache::Http::Code::Bad_Request, json_result.dump());

    }

    void V1_modules_get_handler(const Rest::Request &request, Http::ResponseWriter response) {

//        V1IntegrationAdminTokenPostBody PostBody;
        std::string _acl_resource = "Magento_Backend::admin";
        try {
            this->admin_acl_check(request, _acl_resource);
            // nlohmann::json request_body = nlohmann::json::parse(request.body());
            // PostBody.fromJson(request_body);
            this->V1_modules_get(request, response);
        } catch (std::runtime_error &e) {
            //send a 400 error
            Error_response error;
            error.setMessage(e.what());
            response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
            response.send(Pistache::Http::Code::Bad_Request, error.toJson().dump());
            return;
        } catch (exception &e) {
            Error_response error;
            error.setMessage(e.what());
            response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
            response.send(Pistache::Http::Code::Bad_Request, error.toJson().dump());
            return;
        } catch (Error_response &e) {
            response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
            response.send(e.getHttpCode(), e.toJson().dump());
        }
    }

    void V1_modules_get(const Rest::Request &request, Pistache::Http::ResponseWriter &response) {

        bool validAdmin = false;
        int adminId = 0;

        std::list<std::string> result;

        auto sess = dbConnection->getSession();
        {
            auto db = sess.getDefaultSchema();
            auto store_table = db.getTable("setup_module");
            //  select password from admin_user where username = ? limit 1;
            auto res = store_table.select("module").execute();
            auto data = res.fetchAll();
            const Columns &columns = res.getColumns();
            for (Row row : data) {
                result.push_back(std::string(row[0]));
            }
        }

        nlohmann::json json_result(result);
        response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
        response.send(Pistache::Http::Code::Bad_Request, json_result.dump());

    }

    std::string _createAdminToken(int userId) {
        auto token = encryptor->random_string(32);
        auto secret = encryptor->random_string(32);

        auto sess = dbConnection->getSession();
        {
            auto db = sess.getDefaultSchema();
            auto token_table = db.getTable("oauth_token");
            //  select password from admin_user where username = ? limit 1;
            auto res = token_table.insert("admin_id", "type", "token", "secret", "callback_url", "user_type")
                    .values(userId, "access", token, secret, "", 2)
                    .execute();
        }
        return token;
    }


    std::shared_ptr<Http::Endpoint> httpEndpoint;
    sharedClient dbConnection;
    sharedEncryptor encryptor;
    Rest::Router router;
};

int main(int argc, char *argv[]) {
    Pistache::Port port(8080);
    std::string dbConfigStr;
    dbConfigStr = std::getenv("M2_MYSQL_CONNECTION");


    int thr = 2;

    if (argc >= 2) {
        port = std::stol(argv[1]);

        if (argc >= 3)
            thr = std::stol(argv[2]);
    }

    cout << "Cores = " << hardware_concurrency() << endl;
    cout << "Using " << thr << " threads" << endl;
    cout << "CWD: " << fs::current_path() << endl;

    auto encryption_key = std::string(std::getenv("M2_ENCRYPTION_KEY"));
    auto *enc = new m2_encryptor("");
    sharedEncryptor encryptorPtr = sharedEncryptor(enc);

    auto connection_string = dbConfigStr; //std::string(std::getenv("M2_MYSQL_CONNECTION")); // "root:123123qa@172.17.0.3/b12_06"

    cout << connection_string << endl;
    auto *dbConnection = new Client(connection_string, ClientOption::POOL_MAX_SIZE, thr);
    sharedClient ClientPtr = sharedClient(dbConnection);

    auto sess = dbConnection->getSession();
    {
        auto res = sess.sql("select count(1) from core_config_data;").execute();
        Row data = res.fetchOne();
        cout << "Mysql Connection check: " << data[0] << endl;
    }
//
//    auto res_sha = enc->validateHash(
//            "123123qa",
//            "0db6a1f30f2b53aad28e30690a159a35f398c1c7478929c57fb24e8fa6dd4bbd:salt:1");
//    cout << "Hash validation sha256:" << res_sha << endl;

    Pistache::Address addr(Pistache::Ipv4::any(), port);

    auto opts = Pistache::Http::Endpoint::options()
            .threads(1);
    opts.flags(Tcp::Options::ReuseAddr);

    Pistache::Http::Endpoint server(addr);
    MG_M2_API_point stats(addr);

    stats.init(ClientPtr, encryptorPtr, thr);

    stats.start();

}
