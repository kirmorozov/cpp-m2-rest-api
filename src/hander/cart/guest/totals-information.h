namespace Cart::Guest {

    class Handler {

    public:
        App::Core* app;
        Handler(App::Core* initialApp) {
            app = initialApp;
        }

        void handleTotals(const Rest::Request &request, Http::ResponseWriter response) {
            auto cart = request.param(":cart").as<std::string>();
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