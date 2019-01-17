//
// Created by jnhkm on 2019-01-16.
//

#ifndef WEBSOCKETNETWORKING_USER_H
#define WEBSOCKETNETWORKING_USER_H

#include <string>

namespace model {
    enum class Role {Admin, Builder, Default};

    /*
     * Must use the automatic lifetime constructors instead of assigning to the heap
     * Using the User userVar{"name", 123}; for example.
     */
    class User {
    public:
        User(std::string username, int password);

        User(std::string username, int password, Role role);

        std::string getUsername();

        void setUsername();

        int getPassword();

        void setPassword();

        Role getRole();

        void setRole();

        double getHealth();

        void setHealth();
    private:
        std::string username;

        int password;

        Role role;

        double health;
    };
}

#endif //WEBSOCKETNETWORKING_USER_H

