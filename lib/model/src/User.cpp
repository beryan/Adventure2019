//
// Created by jnhkm on 2019-01-22.
//

#include "User.h"

using model::User;

namespace model {
    User::User(int id) :
        id(id),
        keywords({}),
        description({}),
        health(STARTING_HEALTH)
        {}

    User::User(int id, std::vector<std::string> keys, std::vector<std::string> desc) :
        id(id),
        keywords(std::move(keys)),
        description(std::move(desc)),
        health(STARTING_HEALTH)
        {}

    int User::getId() {
        return id;
    }

    void User::setId(int id) {
        this->id = id;
    }

    void User::addKeyword(std::string key) {
        keywords.push_back(std::move(key));
    }

    void User::removeKeyword(int index) {
        keywords.erase(keywords.begin() + index);
    }

    std::vector<std::string> User::getKeywords() {
        return keywords;
    }

    double User::getHealth() {
        return health;
    }

    void User::setHealth(double health) {
        this->health = health;
    }

    std::vector<std::string> User::getDescription() {
        return description;
    }

    void User::setDescription(std::string desc) {
        description.push_back(std::move(desc));
    }
}