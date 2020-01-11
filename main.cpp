
/*
   Mathieu Stefani, 07 février 2016
   Example of a REST endpoint with routing
*/

#include <algorithm>

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>

#include <mysqlx/xdevapi.h>

using namespace std;
using namespace Pistache;
using namespace mysqlx;


void printCookies(const Http::Request& req) {
    auto cookies = req.cookies();
    std::cout << "Cookies: [" << std::endl;
    const std::string indent(4, ' ');
    for (const auto& c: cookies) {
        std::cout << indent << c.name << " = " << c.value << std::endl;
    }
    std::cout << "]" << std::endl;
}

namespace Generic {

    void handleReady(const Rest::Request&, Http::ResponseWriter response) {

        response.send(Http::Code::Ok, "1");
    }

}

class StatsEndpoint {
public:
    StatsEndpoint(Address addr)
            : httpEndpoint(std::make_shared<Http::Endpoint>(addr))
    { }

    void init(size_t thr = 2) {
        auto opts = Http::Endpoint::options()
                .threads(thr);
        httpEndpoint->init(opts);
        setupRoutes();
        cout << "Initialized" << endl;
    }

    void start() {
        httpEndpoint->setHandler(router.handler());
        httpEndpoint->serve();
    }

private:
    void setupRoutes() {
        using namespace Rest;

        Routes::Post(router, "/record/:name/:value?", Routes::bind(&StatsEndpoint::doRecordMetric, this));
        Routes::Get(router, "/value/:name", Routes::bind(&StatsEndpoint::doGetMetric, this));
        Routes::Get(router, "/ready", Routes::bind(&Generic::handleReady));
        Routes::Get(router, "/auth", Routes::bind(&StatsEndpoint::doAuth, this));

    }

    void doRecordMetric(const Rest::Request& request, Http::ResponseWriter response) {
        auto name = request.param(":name").as<std::string>();

        Guard guard(metricsLock);
        auto it = std::find_if(metrics.begin(), metrics.end(), [&](const Metric& metric) {
            return metric.name() == name;
        });

        int val = 1;
        if (request.hasParam(":value")) {
            auto value = request.param(":value");
            val = value.as<int>();
        }

        if (it == std::end(metrics)) {
            metrics.push_back(Metric(std::move(name), val));
            response.send(Http::Code::Created, std::to_string(val));
        }
        else {
            auto &metric = *it;
            metric.incr(val);
            response.send(Http::Code::Ok, std::to_string(metric.value()));
        }

    }

    void doGetMetric(const Rest::Request& request, Http::ResponseWriter response) {
        auto name = request.param(":name").as<std::string>();

        Guard guard(metricsLock);
        auto it = std::find_if(metrics.begin(), metrics.end(), [&](const Metric& metric) {
            return metric.name() == name;
        });

        if (it == std::end(metrics)) {
            response.send(Http::Code::Not_Found, "Metric does not exist");
        } else {
            const auto& metric = *it;
            response.send(Http::Code::Ok, std::to_string(metric.value()));
        }

    }

    void doAuth(const Rest::Request& request, Http::ResponseWriter response) {
        printCookies(request);
        response.cookies()
                .add(Http::Cookie("lang", "en-US"));

        response.send(Http::Code::Ok);
    }

    class Metric {
    public:
        Metric(std::string name, int initialValue = 1)
                : name_(std::move(name))
                , value_(initialValue)
        { }

        int incr(int n = 1) {
            int old = value_;
            value_ += n;
            return old;
        }

        int value() const {
            return value_;
        }

        std::string name() const {
            return name_;
        }
    private:
        std::string name_;
        int value_;
    };

    typedef std::mutex Lock;
    typedef std::lock_guard<Lock> Guard;
    Lock metricsLock;
    std::vector<Metric> metrics;

    std::shared_ptr<Http::Endpoint> httpEndpoint;
    Rest::Router router;
};

int main(int argc, char *argv[]) {
    Port port(8080);

    int thr = 2;

    if (argc >= 2) {
        port = std::stol(argv[1]);

        if (argc >= 3)
            thr = std::stol(argv[2]);
    }

    Client dbConnection{"root:123123qa@172.17.0.2/b12_06", ClientOption::POOL_MAX_SIZE, thr} ;

    Session sess = dbConnection.getSession();
    {
        auto res = sess.sql("select entity_id from sales_order order by entity_id desc;").execute();
        Row data = res.fetchOne();
        cout << "Result: " << data[0] << endl;
    }

    Address addr(Ipv4::any(), port);

    cout << "Cores = " << hardware_concurrency() << endl;
    cout << "Using " << thr << " threads" << endl;

    StatsEndpoint stats(addr);

    stats.init(thr);
    stats.start();

}

/*
#include <iostream>
#include <fstream>
#include <sstream>

#include "pistache/endpoint.h"

using namespace Pistache;

class HelloHandler : public Http::Handler {
public:

HTTP_PROTOTYPE(HelloHandler)

    void onRequest(const Http::Request& request, Http::ResponseWriter response) override{
        UNUSED(request);
        response.send(Pistache::Http::Code::Ok, "Hello World\n");
    }
};

using string = std::string;

string readFile(string path)
{
    string result;
    std::ifstream ifs(path.c_str(), std::ios::binary);
    std::stringstream ss;

    if (!ifs.is_open()) {
        // Unable to read file
        result.clear();
        return result;
    }
    else if (ifs.eof()) {
        result.clear();
    }
    else {
        ifs.seekg(0);
    }
    ss << ifs.rdbuf() << '\0';

    result = ss.str();
    return result;
}

int main(int argc, char *argv[]) {
    string hello = "OK hi";

    string config_path = "app/etc/env.php";

    if (argc == 3) config_path = argv[2];
    string config = readFile(config_path);
    std::cout << "Hello, World!" << hello << config << std::endl;

//    Pistache::Address addr(Pistache::Ipv4::any(), Pistache::Port(9080));
//    auto opts = Pistache::Http::Endpoint::options()
//            .threads(1);
//
//    Http::Endpoint server(addr);
//    server.init(opts);
//    server.setHandler(Http::make_handler<HelloHandler>());
//    server.serve();

    return 0;
}
*/
