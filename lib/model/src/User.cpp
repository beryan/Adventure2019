//
// Created by jnhkm on 2019-01-16.
//

#include "User.h"

using model::User;

namespace model {
    //TODO: Change values in the future to accommodate for different roles
    enum class Role {Admin, Builder, Default};

    class Player : public User {
    public:
        Player(int id, std::string username, int password) : User(id), username(username), password(password), role(Role::Default){}

        std::string getUsername() {return username;};
        void setUsername(std::string username) {this->username = username;};

        int getPassword() {return password;};
        void setPassword(int password) {this->password = password;};

        Role getRole() {return role;};
        void setRole(Role role) {this->role = role;};

    private:
        std::string username;

        int password;

        Role role;
    };

    class NPC : public User {
    public:
        NPC(int id) : User(id), shortdescr(""), longdesc("") {}

        std::string getShortDescription() {return this->short_description;};

        void setShortDescription(std::string desc) {this->short_description = desc;};

        std::string getLongDescription() {return this->long_description;};

        void setLongDescription(std::string desc) {this->long_description = desc;};
    private:
        std::string shortdesc;

        std::string longdesc;

    };
}
