//
// Created by jnhkm on 2019-01-16.
//

#ifndef WEBSOCKETNETWORKING_USER_H
#define WEBSOCKETNETWORKING_USER_H

#include <string>
#include <vector>
#include <utility>

const double STARTING_HEALTH = 100.00;
const double MAX_HEALTH = 100.00;
const int MAX_KEYWORDS = 8;

namespace model {
    class User {
    public:
        User(int id);

        User(int id, std::vector<std::string> keys, std::vector<std::string> desc);

        int getId();

        void setId(int id);

        void addKeyword(std::string key);

        void removeKeyword(int index);

        std::vector<std::string> getKeywords();

        double getHealth();

        void setHealth(double health);

        std::vector<std::string> getDescription();

        void setDescription(std::string desc);

    private:
        int id;

        std::vector<std::string> keywords;

        std::vector<std::string> description;

        double health;
    };

    inline double getMaxHealth() { return MAX_HEALTH; };

    inline double getStartingHealth() { return STARTING_HEALTH; };
}

#endif //WEBSOCKETNETWORKING_USER_H

