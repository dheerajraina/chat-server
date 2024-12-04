#include <crow.h>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <iostream>
#include <bsoncxx/json.hpp>
#include <jsoncpp/json/json.h>
#include <set>
#include <fstream>
#include <sstream>
#include <string>
#include <mongocxx/exception/exception.hpp>
#include "environment/env.h" // env file

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

std::set<crow::websocket::connection *> clients; // temporary

// MongoDB initialization
mongocxx::instance mongo_instance{};
mongocxx::uri mongo_uri(MONGO_URI);
mongocxx::client mongo_client(mongo_uri);

int ping_mongo_connection()
{
        try
        {
                auto admin = mongo_client["admin"];
                auto command = make_document(kvp("ping", 1));
                auto result = admin.run_command(command.view());
                std::cout << bsoncxx::to_json(result) << "\n";
                std::cout << "Pinged your deployment. You successfully connected to MongoDB \n";
        }
        catch (const mongocxx::exception &e)
        {
                std::cerr << "An exception occured: " << e.what() << "\n";
                return EXIT_FAILURE;
        }
}

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

// API: Create user account
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

                                                                            auto db = mongo_client["chat_server"];
                                                                            auto users_collection = db["users"];

                                                                            // Check if user already exists
                                                                            auto find_result = users_collection.find_one(make_document(kvp("username", body["username"].s())));
                                                                            if (find_result)
                                                                            {
                                                                                    return crow::response(409, "User already exists");
                                                                            }

                                                                            // Insert new user
                                                                            auto insert_result =users_collection.insert_one(make_document(
                                                                            kvp("username",body["username"].s()),
                                                                            kvp("password", body["password"].s()) // hash the password
                                                                            ));
                                                                        if(insert_result){
                                                                                return crow::response(200, "User registered");
                                                                            }

                                                                }
                                                                    catch (const std::exception &e)
                                                                    {
                                                                            std::cerr << e.what() << '\n';

                                                                    } });
}

// WebSocket: Handle messaging
void websocket_chat(crow::SimpleApp &app)
{
        CROW_ROUTE(app, "/ws").websocket(&app).onopen([](crow::websocket::connection &conn)
                                                      {
                                                              std::cout << "WebSocket connection opened\n";
                                                              clients.insert(&conn); })

            .onmessage([](crow::websocket::connection &conn, const std::string &message, bool is_binary)
                       {
                               // Parse message (example: {"from": "user1", "to": "user2", "content": "Hello"})
                               Json::Reader reader;
                               Json::Value msg_json;
                               if (!reader.parse(message, msg_json))
                               {
                                       conn.send_text("Invalid message format");
                                       return;
                               }

                               std::string from = msg_json["from"].asString();
                               std::string to = msg_json["to"].asString();
                               std::string content = msg_json["content"].asString();

                               for (auto client : clients)
                               {
                                       client->send_text(message);
                               }

                       })
            .onclose([](crow::websocket::connection &conn, const std::string &reason)
                     { std::cout << "WebSocket connection closed: " << reason << "\n"; });
}


int main()
{

        try
        {
                crow::SimpleApp app;

                app.loglevel(crow::LogLevel::Debug);

                ping_mongo_connection();

                // Register APIs and WebSocket
                register_account(app);
                websocket_chat(app);

                CROW_ROUTE(app, "/web")
                    .methods(crow::HTTPMethod::GET)(
                        []()
                        {
                                const std::string file_path = "login_signup/index.html";

                                CROW_LOG_DEBUG << "Serving file: " << file_path;
                                auto page = crow::mustache::load(file_path);
                                return page.render();
                        });

                // Start server
                app.port(8080).multithreaded().run();
        }
        catch (const std::exception &ex)
        {
                std::cerr << "Error: " << ex.what() << std::endl;
        }
}
