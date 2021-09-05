#include <algorithm>
#include <thread>
#include <iostream>
#include <fstream>
#include <filesystem>

#include <pistache/common.h>
#include <pistache/http.h>
#include <pistache/http_headers.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>

#include <mysqlx/xdevapi.h>
#include <sw/redis++/redis++.h>

#include "encryptor.h"

#include "nlohmann/json.hpp"
#include "Error-response.h"
#include "Error-response.cpp"
#include "ModelBase.h"
#include "ModelBase.cpp"
#include "store/V1StoreView.h"
#include "store/V1StoreView.cpp"
#include "store/V1StoreGroup.h"
#include "store/V1StoreGroup.cpp"

using namespace Pistache;
using namespace Pistache::Http;
using namespace Pistache::Rest;

using nlohmann::json;
using nlohmann::json_pointer;
using namespace mysqlx;

using namespace io::swagger::server::model;

using m2_encryptor = mg::m2::Encryptor;