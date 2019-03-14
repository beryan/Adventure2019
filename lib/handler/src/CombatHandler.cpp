//
// Created by jnhkm on 2019-03-10.
//

#include "CombatHandler.h"
#include <iostream>

using action::CombatHandler;

namespace action {
    CombatHandler::CombatHandler() :
        active_combats{},
        logic(active_combats){}

    void CombatHandler::enterCombat(const Character &attacker, const Character &defender) {
        if (logic.canEnterCombat(attacker.getId(), defender.getId())) {
            active_combats.emplace_back(CombatState{attacker.getId(), defender.getId()});
        }
    }

    void CombatHandler::exitCombat(const Character &attacker, const Character &defender) {
        if (logic.canExitCombat(attacker.getId(), defender.getId())) {
            auto it = std::find(
                    active_combats.begin(),
                    active_combats.end(),
                    CombatState{attacker.getId(), defender.getId()});

            if (it != active_combats.end()) {
                active_combats.erase(it);
            }
        }
    }

    void CombatHandler::attack(const Character &attacker, Character &defender) {
        if (logic.canAttackTarget(attacker.getId(), defender.getId())) {
            int newHealth = defender.getHealth() - BASE_DAMAGE;
            defender.setHealth(std::max(newHealth, logic::DEFAULT_MIN_HEALTH));
        }
    }

    void CombatHandler::heal(const Character &healer, Character &target) {
        if (logic.canHealTarget(healer.getId(), target.getId())) {
            int newHealth = target.getHealth() + BASE_HEAL;
            target.setHealth(std::min(newHealth, logic::DEFAULT_MAX_HEALTH));
        }
    }

    bool CombatHandler::isInCombat(const Character &attacker, const Character &defender) {
        bool result = false;

        // Can enter combat returns false if the combat state exists
        if (!logic.canEnterCombat(attacker.getId(), defender.getId())) {
            result = true;
        }

        return result;
    }
}