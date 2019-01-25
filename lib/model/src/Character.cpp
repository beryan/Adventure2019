//
// Created by Stephen Wanhella on 2019-01-19.
//

#include "Character.h"

namespace model {

    Character::Character(int id) :
        id(id),
        role(Role::Default),
        health(Character::STARTING_HEALTH)
        {}

    int Character::getId() const {
        return this->id;
    }

    void Character::setId(int id) {
        this->id = id;
    }

    double Character::getHealth() const {
        return this->health;
    }

    void Character::setHealth(double health) {
        if (health > Character::MAX_HEALTH) {
            this->health = Character::MAX_HEALTH;
        } else {
            this->health = health;
        }
    }
}