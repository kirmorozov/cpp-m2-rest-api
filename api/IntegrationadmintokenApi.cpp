/**
* MG M2 Lightning API
* Magento 2.3 API as standalone server
*
* OpenAPI spec version: 0.1.0
* Contact: apiteam@swagger.io
*
* NOTE: This class is auto generated by the swagger code generator program.
* https://github.com/swagger-api/swagger-codegen.git
* Do not edit the class manually.
*/

#include "IntegrationadmintokenApi.h"

namespace io {
namespace swagger {
namespace server {
namespace api {

using namespace io::swagger::server::model;

IntegrationadmintokenApi::IntegrationadmintokenApi(Pistache::Address addr)
    : httpEndpoint(std::make_shared<Pistache::Http::Endpoint>(addr))
{ };

void IntegrationadmintokenApi::init(size_t thr = 2) {
    auto opts = Pistache::Http::Endpoint::options()
        .threads(thr)
        .flags(Pistache::Tcp::Options::InstallSignalHandler);
    httpEndpoint->init(opts);
    setupRoutes();
}

void IntegrationadmintokenApi::start() {
    httpEndpoint->setHandler(router.handler());
    httpEndpoint->serve();
}

void IntegrationadmintokenApi::shutdown() {
    httpEndpoint->shutdown();
}

void IntegrationadmintokenApi::setupRoutes() {
    using namespace Pistache::Rest;

    Routes::Post(router, base + "/integration/admin/token", Routes::bind(&IntegrationadmintokenApi::V1_integration_admin_token_post_handler, this));

    // Default handler, called when a route is not found
    router.addCustomHandler(Routes::bind(&IntegrationadmintokenApi::integrationadmintoken_api_default_handler, this));
}

void IntegrationadmintokenApi::V1_integration_admin_token_post_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {

    // Getting the body param
    V1IntegrationAdminTokenPostBody v1IntegrationAdminTokenPostBody;
    
    try {
      nlohmann::json request_body = nlohmann::json::parse(request.body());
      v1IntegrationAdminTokenPostBody.fromJson(request_body);
      this->V1_integration_admin_token_post(v1IntegrationAdminTokenPostBody, response);
    } catch (std::runtime_error & e) {
      //send a 400 error
      response.send(Pistache::Http::Code::Bad_Request, e.what());
      return;
    }

}

void IntegrationadmintokenApi::integrationadmintoken_api_default_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    response.send(Pistache::Http::Code::Not_Found, "The requested method does not exist");
}

}
}
}
}

