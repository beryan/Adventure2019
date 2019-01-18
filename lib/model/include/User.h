//
// Created by jnhkm on 2019-01-16.
//

#ifndef WEBSOCKETNETWORKING_USER_H
#define WEBSOCKETNETWORKING_USER_H

#include <string>
#include <list>

static const double STARTING_HEALTH = 100.00;
static const double MAX_HEALTH = 100.00;
static const int MAX_KEYWORDS = 8;

namespace model {
    //TODO: Change values to more appropriate values
    enum class Race {Human, Weeb, Furries};

    class User {
    public:
        inline User(int id) : id(id), race(Race::Human), health(STARTING_HEALTH), description("") {}

        inline int getId() {return id;};
        inline void setId(int id) {this->id = id;};

        inline int addKeyword(std::string key) {
            if (this->keywords.size() < MAX_KEYWORDS) {keywords.push_back(key);}
        };
        inline void removeKeyword(std::string key) {
            keywords.remove_if([](std::string i) {return i.compare(key)} == 0);
        }

        inline std::list getKeywords() {return keywords;};

        inline double getHealth() {return health;};
        inline void setHealth(double health) {this->health = health;};

        inline std::string getDescription() {return description;};
        inline void setDescription(std::string description) {this->description = description;};

    private:
        int id;

        std::list<std::string> keywords;

        double health;

        std::string description;
    };

    inline double getMaxHealth() {return MAX_HEALTH;};
    inline double getStartingHealth() {return STARTING_HEALTH;};
}

#endif //WEBSOCKETNETWORKING_USER_H

