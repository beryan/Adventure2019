//
// Created by jnhkm on 2019-01-16.
//

#include "User.h"

using model::User;
using model::Race;

namespace model {
    //TODO: Change values in the future to accommodate for different roles
    enum class Role {Admin, Builder, Default};

    class Player : public User {
    public:
        Player(int id, std::string username, int password) : User(id), username(username), password(password){}

        std::string getUsername() {return username;};
        void setUsername(std::string username) {this->username = username;};

        int getPassword() {return password;};
        void setPassword(int password) {this->password = password;};

        Role getRole() {return role;};
        void setRace(Role role) {this->role = role;};

    private:
        std::string username;

        int password;

        Role role;
    };

    class NPC : public User {

    };
}
