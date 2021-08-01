namespace Generic {

    void handleReady(const Rest::Request &, Http::ResponseWriter response) {
        auto thr_id = std::this_thread::get_id();
        std::stringstream respBuffer;
        respBuffer << "Current Thread: " << thr_id;
        response.send(Http::Code::Ok, respBuffer.str());
    }

}