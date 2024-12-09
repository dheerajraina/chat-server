#include "modules/user/routes/get_contacts_route.h"
#include "modules/databases/mongodb_client.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

void getContacts(crow::SimpleApp &app)
{
        CROW_ROUTE(app, "/get-contacts")
            .methods("GET"_method)([](const crow::request &req)
                                   {
					try{


						// TODO :- Check if user is admin

						auto filter = std::nullopt;


						auto result = find_multiple_records("users",filter);
						if (result.size()==0)
						{
							return crow::response(400, "No contacts found!!");

						}

                                                crow::json::wvalue response;
                                                response["data"]=result;
                                                return crow::response(200,response);

					}
					catch (const std::exception &e)
					{
						std::cerr << e.what() << '\n';

					} });
}
