namespace Cart::Guest {

    class Handler {

    public:
        std::string sessionCookieName = "PHPSESSID";
        App::Core* app;
        Handler(App::Core* initialApp) {
            app = initialApp;
        }

        std::string getSessionCookie(const Rest::Request &request) {
            if (request.cookies().has(sessionCookieName)) {
                return request.cookies().get(sessionCookieName).value;
            }
            return "";
        }

        nlohmann::json getSessionData(const std::string &session_id) {

            nlohmann::json result;
            {
                try {
                    auto raw_data = app->sessionConnection->hget("sess_" + session_id,"data").value_or("");
                    auto data_view = std::string_view(raw_data);
                    std::string data = raw_data;
                    auto type = data_view.substr(0,4);
                    if (type == ":gz:") data = App::Util::Zlib::decompress(raw_data.substr(4));

                    data_view = std::string_view(data);

                    result = App::Util::Php::Serializer::sessionUnserialize(data_view);
                }
                catch(const std::exception &err) {
                    std::cout << err.what() << std::endl;
                }
            }

            // std::cout << result.dump() << std::endl;

            return result;
        }

        void handleTotals(const Rest::Request &request, Http::ResponseWriter response) {
            auto cart = request.param(":cart").as<std::string>();
            if (cart == "mine") {

                auto sessionId = getSessionCookie(request);
                auto sessionData = getSessionData(sessionId);

                nlohmann::json result;
                result[sessionCookieName] = sessionId;
                // load data from session
                response.send(Pistache::Http::Code::Ok, result.dump());
                return;
            }
            {
                auto sess = app->dbConnection->getSession();
                auto db = sess.getDefaultSchema();

                auto quote_id_mask = db.getTable("quote_id_mask");
                auto res = quote_id_mask.select("quote_id")
                        .where("masked_id = :cart")
                        .limit(1)
                        .bind("cart", cart)
                        .execute();

                auto data = res.fetchOne();
                auto passwd = data[0];
                if (data[0].isNull()) {
                    nlohmann::json result;
                    nlohmann::json j_null;
                    result["message"] = "No such entity with %fieldName = %fieldValue";
                    result["parameters"]["fieldName"] = "cartId";
                    result["parameters"]["fieldValue"] = j_null;
                    response.send(Pistache::Http::Code::Not_Found, result.dump());
                    return;
                }
                // TODO: provide real response
                nlohmann::json result;
                response.send(Pistache::Http::Code::Ok, result.dump());
            }
        }
    };
}