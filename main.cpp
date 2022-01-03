
#include "references.hpp"

#include "app.h"
#include "handers.h"

using namespace std;

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

class Service : public App::Core {
public:
    Service(Address addr)
            : httpEndpoint(std::make_shared<Http::Endpoint>(addr)) {}

    void init(dbClient mysql, sessionClient redis, sharedEncryptor enc, int thr = 2) {
        auto opts = Http::Endpoint::options()
                .threads(thr)
                .flags(Tcp::Options::ReuseAddr);
        httpEndpoint->init(opts);
        setupRoutes();
        dbConnection = mysql;
        sessionConnection = redis;
        encryptor = enc;
    }

    void start() {
        httpEndpoint->setHandler(router.handler());
        httpEndpoint->serve();
        cout << "Running" << endl;
    }

protected:
    void setupRoutes() {
        std::string base = "/rest/V1";

        Routes::Get(router, base + "/store/storeViews",
                    Routes::bind(&Service::V1_store_storeViews_get_handler, this));
        Routes::Get(router, base + "/store/storeGroups",
                    Routes::bind(&Service::V1_store_storeGroups_get_handler, this));
        Routes::Get(router, base + "/modules",
                    Routes::bind(&Service::V1_modules_get_handler, this));

        Generic::init(router);
        Integration::init(router, this);
        Cart::init(router, this);
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

    std::shared_ptr<Http::Endpoint> httpEndpoint;
    Rest::Router router;
};

void error_out(std::string message) {
    cout << message << endl;
    exit(100);
}

int main(int argc, char *argv[]) {

    std::string config_name("config.json");
    if (argc >= 2) {
        config_name = argv[1];
    }
    std::ifstream config_file(config_name);
    json jconfig;
    int concurrecy = hardware_concurrency();

    dbClient mysqlConnection;
    sessionClient redisConnection;

    if (config_file.is_open()) {
        jconfig = nlohmann::json::parse(config_file);
        config_file.close();
    } else {
        error_out("Can't open config file.");
    }

    cout << "Using " << concurrecy << " threads" << endl;
    cout << "CWD: " << fs::current_path() << endl;

    {
        auto dbjson = jconfig["/db/connection/default"_json_pointer];
        std::string dbConfigStr = "" +
                      dbjson["username"].get<std::string>() + ":" +
                      dbjson["password"].get<std::string>() + "@" +
                      dbjson["host"].get<std::string>() + "/" +
                      dbjson["dbname"].get<std::string>();

        auto *dbConnection = new Client(dbConfigStr, ClientOption::POOL_MAX_SIZE, concurrecy);
        mysqlConnection = dbClient(dbConnection);
        {
            auto sess = mysqlConnection->getSession();
            auto res = sess.sql("select version(), count(1) from core_config_data;").execute();
            Row data = res.fetchOne();
            cout << "Mysql version: " << data[0] << " Config values: " << data[1] << endl;
        }
    }

    auto encryption_key = jconfig["crypt"]["key"].get<std::string>();
    sharedEncryptor encryptorPtr = std::make_shared<m2_encryptor>(encryption_key);

    auto session_storage = jconfig["session"]["save"].get<std::string>();
    if (session_storage == "redis") {
        sw::redis::ConnectionOptions connection_options;
        connection_options.host = jconfig["session"]["redis"]["host"];  // Required.
        std::string _r_port = jconfig["session"]["redis"]["port"];
        std::string _r_db = jconfig["session"]["redis"]["database"];
        connection_options.port = std::stoi(_r_port); // Optional. The default port is 6379.
        connection_options.db   = std::stoi(_r_db);

        sw::redis::ConnectionPoolOptions pool_opts;
        pool_opts.size = concurrecy;
        pool_opts.wait_timeout = std::chrono::milliseconds(50);
        redisConnection = std::make_shared<sw::redis::Redis>(connection_options, pool_opts);
        {
            auto info_str = redisConnection->info("server");
            auto start_p = info_str.find('\n');
            auto end_p = info_str.find('\n', start_p+1);
            cout << info_str.substr(start_p+1,end_p-start_p-2) << endl;
        }
    }

    int portNum = 8080;
    Pistache::Port port(portNum);
    Pistache::Address addr(Pistache::Ipv4::any(), port);

    auto opts = Pistache::Http::Endpoint::options()
            .threads(concurrecy)
            .flags(Tcp::Options::ReuseAddr);

    Pistache::Http::Endpoint server(addr);
    Service app(addr);

    app.init(mysqlConnection, redisConnection, encryptorPtr, concurrecy);

    app.start();

}
