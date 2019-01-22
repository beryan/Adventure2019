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
        Player(int id, std::string username, int password);

        std::string getUsername();

        void setUsername(std::string username);

        int getPassword();

        void setPassword(int password);

        Role getRole();

        void setRole(Role role);

    private:
        std::string username;

        int password;

        Role role;
    };
}


#endif //WEBSOCKETNETWORKING_PLAYER_H
