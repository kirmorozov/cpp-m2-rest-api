namespace Integration::Admin {

    class Handler {

    public:
        App::Core* app;
        Handler(App::Core* initialApp) {
            app = initialApp;
        }

        void handleToken(const Rest::Request &request, Http::ResponseWriter response) {

            try {
                json request_body = json::parse(request.body());

                bool validAdmin = false;
                int adminId = 0;
                {
                    auto sess = app->dbConnection->getSession();
                    auto db = sess.getDefaultSchema();
                    auto admin_table = db.getTable("admin_user");
                    //  select password from admin_user where username = ? limit 1;
                    auto res = admin_table.select("user_id", "password")
                            .where("username = :user")
                            .limit(1)
                            .bind("user", request_body["username"].get<std::string>())
                            .execute();
                    Row data = res.fetchOne();
                    auto passwd = data[0];
                    if (!data[0].isNull()) {
                        // +70-75 ms extra for request.
                        validAdmin = app->encryptor->validateHash(
                                request_body["password"].get<std::string>(), std::string(data[1]));
                    }
                    if (validAdmin) {
                        adminId = int(data[0]);
                    }
                }
                if (validAdmin) {
                    nlohmann::json result = _createAdminToken(adminId);
                    response.send(Pistache::Http::Code::Ok, result.dump());
                } else {
                    Error_response error;
                    error.setMessage("Access forbidden");
                    error.setCode(int(Pistache::Http::Code::Forbidden));
                    response.send(Pistache::Http::Code::Forbidden, error.toJson().dump());
                }

            } catch (std::runtime_error &e) {
                //send a 400 error
                Error_response error;
                error.setMessage(e.what());
                response.send(Pistache::Http::Code::Bad_Request, error.toJson().dump());
                return;
            } catch (json::exception &e) {
                Error_response error;
                error.setMessage(e.what());
                response.send(Pistache::Http::Code::Bad_Request, error.toJson().dump());
                return;
            }
        }

        std::string _createAdminToken(int userId) {
            auto token = app->encryptor->random_string(32);
            auto secret = app->encryptor->random_string(32);

            {
                auto sess = app->dbConnection->getSession();
                auto db = sess.getDefaultSchema();
                auto token_table = db.getTable("oauth_token");
                //  select password from admin_user where username = ? limit 1;
                auto res = token_table.insert("admin_id", "type", "token", "secret", "callback_url", "user_type")
                        .values(userId, "access", token, secret, "", 2)
                        .execute();
            }
            return token;
        }

    };
}