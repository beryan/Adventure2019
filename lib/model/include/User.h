//
// Created by jnhkm on 2019-01-16.
//

#ifndef WEBSOCKETNETWORKING_USER_H
#define WEBSOCKETNETWORKING_USER_H

#include <string>
#include <vector>
#include <array>
#include <utility>
#include "Item.h"

const double STARTING_HEALTH = 100.00;
const double MAX_HEALTH = 100.00;
const int MAX_KEYWORDS = 8;

using model::Item;
using model::Slot;

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


        std::array<Item*, Slot::Count> getEquipments();

        void equipItem(Item *item);

        std::vector<Item> getInventory();

        void addItem(Item item);

        bool operator==(const User&);

    private:
        int id;

        std::vector<std::string> keywords;

        std::vector<std::string> description;

        std::vector<Item> inventory;

        std::array<Item*, Slot::Count> equipment;

        double health;
    };

  inline double getMaxHealth() { return MAX_HEALTH; };

    inline double getStartingHealth() { return STARTING_HEALTH; };
}

#endif //WEBSOCKETNETWORKING_USER_H

