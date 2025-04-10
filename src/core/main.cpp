#include <crow.h>
#include <iostream>
#include <unordered_map>
#include <mutex>
// #include <bsoncxx/json.hpp>
#include <jsoncpp/json/json.h>
#include <set>
#include <fstream>
#include <sstream>
#include <string>
// #include "modules/databases/mongodb_client.h"
// #include "modules/auth/routes/signup_route.h"
// #include "modules/auth/routes/login_route.h"
// #include "modules/user/routes/get_contacts_route.h"

// using bsoncxx::builder::basic::kvp;
// using bsoncxx::builder::basic::make_document;

std::unordered_map<std::string, crow::websocket::connection *> online_users; // maps _id to socket client
std::set<crow::websocket::connection *> clients; // stores all open clients
std::mutex online_users_mutex;

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

			       // Store in MongoDB
			       //     auto db = mongo_client["chat_app"];
			       //     auto messages = db["messages"];
			       //     auto doc = bsoncxx::builder:: document{}
			       //                << "from" << from
			       //                << "to" << to
			       //                << "content" << content
			       //                << "timestamp" << bsoncxx::types::b_date{std::chrono::system_clock::now()}
			       //                << bsoncxx::builder::stream::finalize;

			       //     messages.insert_one(doc.view());

			       // Push to Redis for quick access
			       //     redis.rpush("chat:" + from + ":" + to, content);
			       //     redis.ltrim("chat:" + from + ":" + to, -10, -1);  // Keep only the last 10 messages

			       // Deliver message if the recipient is online
			       std::lock_guard<std::mutex> lock(online_users_mutex);
			       for (auto client : clients)
			       {
				       if (client != &conn)
				       {
					       client->send_text(message);
				       }
			       }
			       //     if (online_users.find(to) != online_users.end()) {
			       //         online_users[to]->send_text(message);
			       //     }
		       })
	    .onclose([](crow::websocket::connection &conn, const std::string &reason)
		     { std::cout << "WebSocket connection closed: " << reason << "\n"; });
}

std::string load_html(const std::string &file_path)
{
	std::ifstream file(file_path);
	if (!file.is_open())
	{
		return "<h1>Error: Could not open HTML file.</h1>";
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

int main()
{

	try
	{
		crow::SimpleApp app;

		app.loglevel(crow::LogLevel::Debug);

		// init_mongo_connection();
		// ping_mongo_connection();

		// Register APIs and WebSocket
		// register_account(app);
		// login(app);
		// getContacts(app);
		websocket_chat(app);

		CROW_ROUTE(app, "/web")
		    .methods(crow::HTTPMethod::GET)(
			[]()
			{
				//  std::string file_path = "static/" + filename;
				//  res.body("dshduhshd");
				const std::string file_path = "login_signup/index.html";

				CROW_LOG_DEBUG << "Serving file: " << file_path;
				auto page = crow::mustache::load(file_path);
				return page.render();
			});

		CROW_ROUTE(app, "/web/chat")
		([]()
		 {
		         std::string html_content = load_html("build/templates/chat/index.html");
		         return html_content; });

		// Start server
		app.port(8080).multithreaded().run();
	}
	catch (const std::exception &ex)
	{
		std::cerr << "Error: " << ex.what() << std::endl;
	}
}
