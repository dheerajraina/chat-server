#ifndef SIGNUP_ROUTE_H
#define SIGNUP_ROUTE_H

#include <crow.h>
#include <mongocxx/client.hpp>

// API: Create user account
void register_account(crow::SimpleApp &app);
#endif