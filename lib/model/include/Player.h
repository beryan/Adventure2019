//
// Created by jnhkm on 2019-01-18.
//

#ifndef WEBSOCKETNETWORKING_PLAYER_H
#define WEBSOCKETNETWORKING_PLAYER_H

#include "User.h"
#include "Role.h"

namespace model {
    class Player : public User {
    public:
        Player(int id, std::string username, std::string password);

        std::string getUsername();

        void setUsername(std::string username);

        std::string getPassword();

        void setPassword(std::string password);

        Role getRole();

        void setRole(Role role);

    private:
        std::string username;

        std::string password;

        Role role;
    };
}


#endif //WEBSOCKETNETWORKING_PLAYER_H
