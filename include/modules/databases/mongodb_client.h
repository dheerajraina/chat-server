#ifndef MONGODB_CLIENT_H
#define MONGODB_CLIENT_H

#include <iostream>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/exception/exception.hpp>

#include <bsoncxx/json.hpp>
#include <jsoncpp/json/json.h>
#include <mutex>
#include <string.h>

extern std::unique_ptr<mongocxx::instance> mongo_instance;
extern std::unique_ptr<mongocxx::client> mongo_client;

// sw::redis::Redis redis("tcp://127.0.0.1:6379");

void init_mongo_connection();
int ping_mongo_connection();
std::optional<bsoncxx::document::value> find_one_record(std::string collection_name, bsoncxx::document::value filter);
std::optional<mongocxx::v_noabi::result::insert_one> insert_one_record(std::string collection_name, bsoncxx::document::value record);
std::vector<std::string> find_multiple_records(std::string collection_name, std::optional<bsoncxx::document::value> filter);
#endif