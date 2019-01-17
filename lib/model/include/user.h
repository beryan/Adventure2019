//
// Created by jnhkm on 2019-01-16.
//

#ifndef WEBSOCKETNETWORKING_USER_H
#define WEBSOCKETNETWORKING_USER_H

#include <string>

namespace model {
    enum Role {Admin, Builder, Default};

    class User {
    public:
        User(std::string username, int password);
        User(std::string username, int password, Role role);
        ~User();
    private:
        std::string username;
        int password;
        Role role;
        double health;
    };
}

#endif //WEBSOCKETNETWORKING_USER_H
