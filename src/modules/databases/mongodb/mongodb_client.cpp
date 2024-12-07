#include <mongocxx/exception/exception.hpp>
#include "environment/env.h" // env file
#include <bsoncxx/json.hpp>
#include <jsoncpp/json/json.h>
#include "modules/databases/mongodb_client.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

std::unique_ptr<mongocxx::instance> mongo_instance = nullptr;
std::unique_ptr<mongocxx::client> mongo_client = nullptr;

void init_mongo_connection()
{

        try
        {
                mongo_instance = std::make_unique<mongocxx::instance>(); // Initialize the MongoDB driver
                mongocxx::uri mongo_uri(MONGO_URI);
                mongo_client = std::make_unique<mongocxx::client>(mongo_uri);
                std::cout << "Initialised Mongo Client\n";
        }
        catch (const mongocxx::exception &e)
        {
                std::cerr << e.what() << '\n';
        }
}

int ping_mongo_connection()
{

        try
        {
                auto admin = (*mongo_client)["admin"];
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

std::optional<bsoncxx::document::value> find_one_record(std::string collection_name, bsoncxx::document::value filter)
{
        auto db = (*mongo_client)["chat_server"];
        auto collection = db[collection_name];
        auto result = collection.find_one(filter.view());

        return result;
}
std::optional<mongocxx::v_noabi::result::insert_one> insert_one_record(std::string collection_name, bsoncxx::document::value record)
{
        auto db = (*mongo_client)["chat_server"];
        auto collection = db[collection_name];
        auto result = collection.insert_one(record.view());

        return result;
}
