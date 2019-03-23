//
// Created by jnhkm on 2019-03-10.
//

#include "CombatLogic.h"

using logic::CombatLogic;

namespace logic {
    CombatLogic::CombatLogic(std::vector<CombatInstance> &states) :
        states(states) {}

    bool CombatLogic::canEnterCombat(const model::ID &attacker, const model::ID &defender) const {
        return static_cast<bool>(
                std::find(
                        this->states.begin(),
                        this->states.end(),
                        CombatInstance{attacker, defender}) == this->states.end()
                        );
    }

    bool CombatLogic::canExitCombat(const model::ID &attacker, const model::ID &defender) const {
        return static_cast<bool>(
                std::find(
                        this->states.begin(),
                        this->states.end(),
                        CombatInstance{attacker, defender}) != this->states.end()
                        );
    }


    bool CombatLogic::canHealTarget(const model::ID &healer, const model::ID &target) const {
        bool result = true;

        // Heal condition

        return result;
    }

    // These two methods all have the same if-condition for now
    // until we get a clearer idea of the specific conditions
    bool CombatLogic::canAttackTarget(const model::ID &attacker, const model::ID &defender) const {
        bool result = false;

        if (std::find(this->states.begin(), this->states.end(), CombatInstance{attacker, defender}) != this->states.end()) {
            result = true;
        }

        return result;
    }

    bool CombatLogic::canFlee(const model::ID &attacker, const model::ID &defender) const {
        bool result = false;

        if (std::find(this->states.begin(), this->states.end(), CombatInstance{attacker, defender}) != this->states.end()) {
            result = true;
        }

        return result;
    }
}