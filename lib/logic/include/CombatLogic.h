//
// Created by jnhkm on 2019-03-10.
//

#ifndef WEBSOCKETNETWORKING_COMBATLOGIC_H
#define WEBSOCKETNETWORKING_COMBATLOGIC_H

#include "Character.h"

using model::Character;

namespace logic {
    constexpr int DEFAULT_MAX_HEALTH = 100;
    constexpr int DEFAULT_MIN_HEALTH = 0;

    class CombatLogic {
    private:
    public:
        CombatLogic();

        bool canAttackTarget(const Character &attacker, const Character &defender);

        bool canHealTarget(const Character &healer, const Character &target);

        bool canFlee(const Character &character);
    };
};



#endif //WEBSOCKETNETWORKING_COMBATLOGIC_H
