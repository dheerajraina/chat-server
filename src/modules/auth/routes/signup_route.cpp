#include "modules/databases/mongodb_client.h"
#include "modules/auth/routes/signup_route.h"
#include <bsoncxx/json.hpp>
#include <jsoncpp/json/json.h>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

// Utility to validate JSON input
bool is_valid_json(const crow::json::rvalue &body, const std::vector<std::string> &keys)
{
        for (const auto &key : keys)
        {
                if (!body.has(key))
                {
                        return false;
                }
        }
        return true;
}

void register_account(crow::SimpleApp &app)
{

        CROW_ROUTE(app, "/register").methods("POST"_method)([](const crow::request &req)
                                                            {
                                                                    try
                                                                    {
                                                                            auto body = crow::json::load(req.body);

                                                                            if (!is_valid_json(body, {"username", "password"}))
                                                                            {

                                                                                    return crow::response(400, "Invalid input");
                                                                            }

                                                                        auto filter =make_document(kvp("username", body["username"].s()));

                                                                        // Check if user already exists
                                                                        auto find_result=find_one_record("users",filter);

                                                                        if (find_result)
                                                                            {
                                                                                    return crow::response(409, "User already exists");
                                                                            }



                                                                        // Insert new user
                                                                        auto record= make_document(
                                                                        kvp("username",body["username"].s()),
                                                                        kvp("password", body["password"].s()) // hash the password
                                                                        );


                                                                        auto insert_result = insert_one_record("users",record);

                                                                        if(insert_result){
                                                                                return crow::response(200, "User registered");
                                                                        }

                                                                }
                                                                    catch (const std::exception &e)
                                                                    {
                                                                            std::cerr << e.what() << '\n';

                                                                    } });
}
