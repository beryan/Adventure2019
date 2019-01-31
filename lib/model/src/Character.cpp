//
// Created by Stephen Wanhella on 2019-01-19.
//

#include "Character.h"

namespace model {

    Character::Character(model::ID id) :
        id(id),
        role(Role::Default),
        health(Character::STARTING_HEALTH)
        {}

    model::ID Character::getId() const {
        return this->id;
    }

    void Character::setId(model::ID id) {
        this->id = id;
    }

    Role Character::getRole() const {
        return this->role;
    }

    void Character::setRole(Role role) {
        this->role = role;
    }

    double Character::getHealth() const {
        return this->health;
    }

    void Character::setHealth(double health) {
        if (health > Character::MAX_HEALTH) {
            this->health = Character::MAX_HEALTH;
        }
        else if (health < 0){
            this->health = 0;
        }
        else {
            this->health = health;
        }
    }

    double Character::reduceHealth(const double &health) {
        this->health -= health;

        if (this->health < 0)
            this->health = 0;

        return this->health;
    }

    double Character::addHealth(const double &health) {
        this->health += health;

        if (this->health > Character::MAX_HEALTH)
            this->health = Character::MAX_HEALTH;

        return this->health;
    }
}