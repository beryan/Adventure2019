//
// Created by jnhkm on 2019-03-10.
//

#include "CombatLogic.h"

using logic::CombatLogic;

namespace logic {
    CombatLogic::CombatLogic(std::vector<CombatState> &active_combats) {
        this->active_combats = active_combats;
    }

    bool CombatLogic::canEnterCombat(const CombatState &combat) {
        return static_cast<bool>(
                std::find(this->active_combats.begin(), this->active_combats.end(), combat) == this->active_combats.end()
                );
    }

    bool CombatLogic::canExitCombat(const CombatState &combat) {
        return static_cast<bool>(
                std::find(this->active_combats.begin(), this->active_combats.end(), combat) != this->active_combats.end()
        );
    }

    bool CombatLogic::canAttackTarget(const Character &attacker, const Character &defender) {
        bool result = true;

        // Check flags, magic, state, whatever
        // Set result to true if they are able to be attacked

        return result;
    }

    bool CombatLogic::canHealTarget(const Character &healer, const Character &target) {
        bool result = true;

        // Check if healer can heal the target

        return result;
    }

    bool CombatLogic::canFlee(const Character &character) {
        bool result = true;

        // Check if the Character can flee in its current state

        return result;
    }
}