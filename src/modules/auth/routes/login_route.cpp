#include "modules/auth/routes/login_route.h"
#include "modules/databases/mongodb_client.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

// Utility to validate JSON input
static bool is_valid_login_json(const crow::json::rvalue &body, const std::vector<std::string> &keys)
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

void login(crow::SimpleApp &app)
{
	CROW_ROUTE(app, "/login")
	    .methods("POST"_method)([](const crow::request &req)
				    {
					try{
						auto body = crow::json::load(req.body);

						if (!is_valid_login_json(body, {"username", "password"}))
						{

							return crow::response(400, "Invalid input");
						}

						// Check if user exists
						auto filter = make_document(
							kvp("username", body["username"].s())
							);


						auto result = find_one_record("users",filter);
						if (result)
						{
							return crow::response(200, "Successfully logged in");

						}

						return crow::response(409, "Invalid username or password!");

					}
					catch (const std::exception &e)
					{
						std::cerr << e.what() << '\n';

					} });
}
