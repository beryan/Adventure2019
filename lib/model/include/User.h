//
// Created by jnhkm on 2019-01-16.
//

#ifndef WEBSOCKETNETWORKING_USER_H
#define WEBSOCKETNETWORKING_USER_H

#include <string>
#include <vector>
#include <utility>

static const double STARTING_HEALTH = 100.00;
static const double MAX_HEALTH = 100.00;
static const int MAX_KEYWORDS = 8;

namespace model {
    //TODO: Change values to more appropriate values
    enum class Race {Human, Weeb, Furries};

    class User {
    public:
        inline User(int id) : id(id), health(STARTING_HEALTH), description({}) {}

        inline User(int id, std::vector<std::string> keys, std::vector<std::string> desc) :
            id(id), keywords(keys), description(desc) {}

        inline int getId() {return id;};

        inline void setId(int id) {this->id = id;};

        inline void addKeyword(std::string key) {keywords.push_back(std::move(key));};

        inline void removeKeyword(int index) {keywords.erase(keywords.begin() + index);};

        inline std::vector<std::string> getKeywords() {return keywords;};

        inline double getHealth() {return health;};

        inline void setHealth(double health) {this->health = health;};

        inline std::vector<std::string> getDescription() {return this->description;};

        inline void setDescription(std::string desc) {description.push_back(std::move(desc));};

    private:
        int id;

        std::vector<std::string> keywords;

        double health;

        std::vector<std::string> description;
    };

    inline double getMaxHealth() {return MAX_HEALTH;};
    inline double getStartingHealth() {return STARTING_HEALTH;};
}

#endif //WEBSOCKETNETWORKING_USER_H

