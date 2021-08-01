

namespace Generic {

    void handleRest(const Rest::Request &, Http::ResponseWriter response) {
        std::stringstream respBuffer;
        respBuffer << R"xml(<?xml version="1.0"?>
<response>
    <message>Specified request cannot be processed.</message>
</response>
        )xml";
        response.headers()
                .add<Header::ContentType>(MIME(Application, Xml));
        response.send(Http::Code::Bad_Request, respBuffer.str());
    }

}