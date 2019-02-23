//
// Created by jnhkm on 2019-02-23.
//

#include "../include/CharacterLogic.h"

using logic::CharacterLogic;

namespace logic {
    bool CharacterLogic::canReduceHealth(int health, int change) {
        return health - change > 0;
    }

    bool CharacterLogic::canIncreaseHealth(int health, int change) {
        return health + change < Character::MAX_HEALTH;
    }
}