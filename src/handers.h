#include "hander/generic/ready.h"
#include "hander/generic/rest.h"


namespace Generic {
    void init(Router& router) {
        Routes::Get(router, "/_ready", Routes::bind(&handleReady));
        Routes::Get(router, "/rest", Routes::bind(&handleRest));
    }
}