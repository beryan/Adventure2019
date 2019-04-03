//
// Created by jnhkm on 2019-01-16.
//

#ifndef CHARACTER_H
#define CHARACTER_H

#include "Object.h"
#include "Role.h"
#include <string_view>

using model::Role;
namespace model {
    /**
    *  @struct Character
    *
    *  @brief A struct describing a character.
    *
    *  The Character struct contains all necessary information
    *  that must be contained in a character object in the application.
    */
    struct Character {
    public:
        explicit Character(model::ID id);

        model::ID getId() const;

        void setId(model::ID id);

        Role getRole() const;

        void setRole(Role role);

        int getHealth() const;

        void setHealth(int health);

        static constexpr int STARTING_HEALTH = 100;
    private:

        model::ID id;

        Role role;

        int health;
    };
}

#endif //CHARACTER_H