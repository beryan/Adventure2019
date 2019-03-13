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


    bool CombatLogic::canHealTarget(Character &healer, Character &target) {
        bool result = true;

        // Heal condition

        return result;
    }

    // These two methods all have the same if-condition for now
    // until we get a clearer idea of the specific conditions
    bool CombatLogic::canAttackTarget(const CombatState &combat) {
        bool result = true;

        if (std::find(this->active_combats.begin(), this->active_combats.end(), combat) != this->active_combats.end()) {
            result = false;
        }

        return result;
    }

    bool CombatLogic::canFlee(const CombatState &combat) {
        bool result = true;

        if (std::find(this->active_combats.begin(), this->active_combats.end(), combat) != this->active_combats.end()) {
            result = false;
        }

        return result;
    }
}