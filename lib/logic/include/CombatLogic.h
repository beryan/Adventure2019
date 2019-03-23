//
// Created by jnhkm on 2019-03-10.
//

#ifndef COMBATLOGIC_H
#define COMBATLOGIC_H

#include "Character.h"
#include "CombatInstance.h"

using model::Character;
using model::CombatInstance;

namespace logic {
    constexpr int DEFAULT_MAX_HEALTH = 100;
    constexpr int DEFAULT_MIN_HEALTH = 0;

    class CombatLogic {
    private:
        std::vector<CombatInstance> &states;
    public:
        explicit CombatLogic(std::vector<CombatInstance> &states);

        /**
         * @param combat state to be checked in active_combats
         * @return False if state already exists between two characters in the
         * Combat param, True if not.
         */
        bool canEnterCombat(const model::ID &attacker, const model::ID &defender) const;

        /**
         *
         * @param combat state to be checked in active_combats
         * @return True if the state already exists between two characters in the Combat
         * param, False if not.
         */
        bool canExitCombat(const model::ID &attacker, const model::ID &defender) const;

        /**
         *
         * @param healer that will use the ability
         * @param target that the heal will affect
         * @return Returns True if the Target can be healed, False if not
         */
        bool canHealTarget(const model::ID &healer, const model::ID &target) const;

        /**
         *
         * @param combat state to check in active_combats
         * @return Returns True if the Combat param exists in active_combats, False if not
         */
        bool canAttackTarget(const model::ID &attacker, const model::ID &defender) const;

        // TODO: Complete implementation detail
        bool canFlee(const model::ID &attacker, const model::ID &defender) const;
    };
};



#endif //COMBATLOGIC_H
