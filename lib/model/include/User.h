//
// Created by jnhkm on 2019-01-16.
//

#ifndef WEBSOCKETNETWORKING_USER_H
#define WEBSOCKETNETWORKING_USER_H

#include <string>

#define STARTING_HEALTH 100.00

namespace model {
    //TODO: Change values to more appropriate values
    enum class Race {Human, Weeb, Furries};

    class User {
    public:
        User(int id) : id(id), race(Race::Human), health(STARTING_HEALTH), description("") {}

        int getId() {return id;};
        void setId(int id) {this->id = id;};

        Race getRace() {return race;};
        void setRace(Race race) {this->race = race;};

        double getHealth() {return health;};
        void setHealth(double health) {this->health = health;};

        std::string getDescription() {return description;};
        void setDescription(std::string description) {this->description = description;};

    private:
        int id;

        Race race;

        double health;

        std::string description;
    };
}

#endif //WEBSOCKETNETWORKING_USER_H

