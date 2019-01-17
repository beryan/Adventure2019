//
// Created by jnhkm on 2019-01-16.
//

#include "User.h"

using model::User;
using model::Role;

namespace model {
    User::User(std::string username, int password) : username(username), password(password) {}

    User::User(std::string username, int password, Role role) : username(username), password(password), role(role){}


}
