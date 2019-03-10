//
// Created by jnhkm on 2019-02-23.
//

#include "../include/CharacterLogic.h"

using logic::CharacterLogic;

namespace logic {
    CharacterLogic::CharacterLogic() {}

    bool CharacterLogic::canReduceHealth(int health, int change) {
        return health - change > DEFAULT_MIN_HEALTH;
    }

    bool CharacterLogic::canIncreaseHealth(int health, int change) {
        return health + change < DEFAULT_MAX_HEALTH;
    }
}