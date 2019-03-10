//
// Created by jnhkm on 2019-02-23.
//

#include "../include/CharacterLogic.h"

using logic::CharacterLogic;

namespace logic {
    CharacterLogic::CharacterLogic() :
        minHealth(DEFAULT_MIN_HEALTH),
        maxHealth(DEFAULT_MAX_HEALTH) {}

    CharacterLogic::CharacterLogic(int minHealth, int maxHealth) :
        minHealth(minHealth),
        maxHealth(maxHealth) {}

    bool CharacterLogic::canReduceHealth(int health, int change) {
        return health - change > minHealth;
    }

    bool CharacterLogic::canIncreaseHealth(int health, int change) {
        return health + change < maxHealth;
    }
}