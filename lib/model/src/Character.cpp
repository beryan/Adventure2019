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

    int Character::getHealth() const {
        return this->health;
    }

    void Character::setHealth(int health) {
        this->health = health;
    }
}