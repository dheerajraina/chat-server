#include <crow.h>
#include <iostream>
#include <jsoncpp/json/json.h>
#include <set>
#include <fstream>
#include <sstream>
#include <string>

std::set<crow::websocket::connection *> clients; // temporary

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

                               // broadcasting message to all the connected clients
                               for (auto client : clients)
                               {
                                       client->send_text(message);
                               } })
            .onclose([](crow::websocket::connection &conn, const std::string &reason)
                     { std::cout << "WebSocket connection closed: " << reason << "\n"; });
}

int main()
{
        try
        {
                crow::SimpleApp app;

                app.loglevel(crow::LogLevel::Debug);

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
