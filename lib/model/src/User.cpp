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
        NPC(int id) : User(id), shortdesc({}), longdesc({}) {}

        NPC(int id, std::vector<std::string> keys, std::vector<std::string> desc, std::string shdesc, std::vector<std::string> lngdesc) :
            User(id, keys, desc), shortdesc(shdesc), longdesc(lngdesc) {}

        std::string getShortDescription() {return this->shortdesc;};

        void setShortDescription(std::string desc) {shortdesc.assign(desc);};

        std::vector<std::string> getLongDescription() {return this->longdesc;};

        void addLongDescription(std::string desc) {longdesc.push_back(std::move(desc));};
    private:
        std::string shortdesc;

        std::vector<std::string> longdesc;

    };
}
