//
// Created by jnhkm on 2019-02-23.
//

#ifndef CHARACTERLOGIC_H
#define CHARACTERLOGIC_H

#include "Character.h"

using model::Character;

namespace logic {
    constexpr int DEFAULT_MIN_HEALTH = 0;
    constexpr int DEFAULT_MAX_HEALTH = 100;

    class CharacterLogic {
    private:
    public:
        CharacterLogic();

        bool canReduceHealth(int health, int change);

        bool canIncreaseHealth(int health, int change);
    };
}



#endif //CHARACTERLOGIC_H
