#include "hander/generic/ready.h"
#include "hander/generic/rest.h"
#include "hander/integration/admin/token.h"
#include "hander/cart/guest/totals-information.h"
#include "hander/admin/modules.h"


namespace Generic {
    void init(Router& router) {
        Routes::Get(router, "/_ready", Routes::bind(&handleReady));
        Routes::Get(router, "/rest", Routes::bind(&handleRest));
    }
}

namespace Integration {
    void init(Router& router, App::Core* app) {
        auto handler = std::make_shared<Admin::Handler>(app);
        Routes::Post(router, "/rest/V1/integration/admin/token",
                     Routes::bind(&Admin::Handler::handleToken,handler));
    }
}

namespace Cart {
    void init(Router& router, App::Core* app) {
        auto handler = std::make_shared<Guest::Handler>(app);
        Routes::Post(router, "/rest/:store/V1/guest-carts/:cart/totals-information",
                     Routes::bind(&Guest::Handler::handleTotals, handler));
        Routes::Post(router, "/rest/V1/guest-carts/:cart/totals-information",
                     Routes::bind(&Guest::Handler::handleTotals, handler));

    }
}

namespace Admin {
    void init(Router& router, App::Core* app) {
//        auto handler = std::make_shared<Guest::Handler>(app);
//        Routes::Post(router, "/rest/:store/V1/guest-carts/:cart/totals-information",
//                     Routes::bind(&Guest::Handler::handleTotals, handler));
//        Routes::Post(router, "/rest/V1/guest-carts/:cart/totals-information",
//                     Routes::bind(&Guest::Handler::handleTotals, handler));

    }
}