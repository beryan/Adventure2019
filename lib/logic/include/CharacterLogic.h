//
// Created by jnhkm on 2019-02-23.
//

#ifndef CHARACTERLOGIC_H
#define CHARACTERLOGIC_H

#include "Character.h"

using model::Character;

namespace logic {
    class CharacterLogic {
    private:
        CharacterLogic();
    public:
        static bool canReduceHealth(int health, int change);

        static bool canIncreaseHealth(int health, int change);
    };
}



#endif //CHARACTERLOGIC_H
