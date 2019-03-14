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

        /**
         * @param combat state to be checked in active_combats
         * @return False if state already exists between two characters in the
         * Combat param, True if not.
         */
        bool canEnterCombat(const CombatState &combat);

        /**
         *
         * @param combat state to be checked in active_combats
         * @return True if the state already exists between two characters in the Combat
         * param, False if not.
         */
        bool canExitCombat(const CombatState &combat);

        /**
         *
         * @param healer that will use the ability
         * @param target that the heal will affect
         * @return Returns True if the Target can be healed, False if not
         */
        bool canHealTarget(Character &healer, Character &target);

        /**
         *
         * @param combat state to check in active_combats
         * @return Returns True if the Combat param exists in active_combats, False if not
         */
        bool canAttackTarget(const CombatState &combat);

        // TODO: Complete implementation detail
        bool canFlee(const CombatState &combat);
    };
};



#endif //WEBSOCKETNETWORKING_COMBATLOGIC_H
