//
// Created by jnhkm on 2019-03-10.
//

#ifndef WEBSOCKETNETWORKING_COMBATLOGIC_H
#define WEBSOCKETNETWORKING_COMBATLOGIC_H

#include "Character.h"
#include "CombatState.h"

using model::Character;
using model::CombatState;

namespace logic {
    constexpr int DEFAULT_MAX_HEALTH = 100;
    constexpr int DEFAULT_MIN_HEALTH = 0;

    class CombatLogic {
    private:
        std::vector<CombatState> &active_combats;
    public:
        CombatLogic(std::vector<CombatState> &active_combats);

        bool canEnterCombat(const CombatState &combat);

        bool canExitCombat(const CombatState &combat);

        bool canHealTarget(Character &healer, Character &target);

        bool canAttackTarget(const CombatState &combat);

        bool canFlee(const CombatState &combat);
    };
};



#endif //WEBSOCKETNETWORKING_COMBATLOGIC_H
