//
// Created by jnhkm on 2019-01-16.
//

#ifndef CHARACTER_H
#define CHARACTER_H

#include "Object.h"
#include <string_view>

namespace model {
    /**
    *  @struct Character
    *
    *  @brief A struct describing a character.
    *
    *  The Character struct contains all necessary information
    *  that must be contained in a character object in the application.
    */

    enum Role { Admin, Builder, Default };

    struct Character {
    public:
        explicit Character(model::ID id);

        model::ID getId() const;

        void setId(model::ID id);

        Role getRole() const;

        void setRole(Role role);

        double getHealth() const;

        void setHealth(double health);

        double reduceHealth(const double &health);

        double addHealth(const double &health);

        static constexpr double STARTING_HEALTH = 100.00;

        static constexpr double MAX_HEALTH = 100.00;

    private:
        model::ID id;

        Role role;

        double health;
    };
}

#endif //CHARACTER_H