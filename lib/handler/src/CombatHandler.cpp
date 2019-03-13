//
// Created by jnhkm on 2019-03-10.
//

#include "CombatHandler.h"

using action::CombatHandler;

namespace action {
    CombatHandler::CombatHandler() {}

    void CombatHandler::enterCombat(const Character &attacker, const Character &defender) {
        CombatState combat{attacker.getId(), defender.getId()};

        if (logic.canEnterCombat(combat)) {
            active_combats.push_back(std::move(combat));
        }
    }

    void CombatHandler::exitCombat(Character &attacker, Character &defender) {
        CombatState combat{attacker.getId(), defender.getId()};

        if (logic.canExitCombat(combat)) {
            active_combats.erase(
                    std::remove_if(
                            active_combats.begin(),
                            active_combats.end(),
                            [&combat](const CombatState &state) {return combat == state;})
                            );
        }
    }

    void CombatHandler::attack(Character &attacker, Character &defender) {
        CombatState combat{attacker.getId(), defender.getId()};

        if (logic.canAttackTarget(combat)) {
            int newHealth = defender.getHealth() - BASE_DAMAGE;
            defender.setHealth(std::max(newHealth, logic::DEFAULT_MIN_HEALTH));
        }
    }

    void CombatHandler::heal(Character &healer, Character &target) {
        if (logic.canHealTarget(healer, target)) {
            int newHealth = target.getHealth() + BASE_HEAL;
            target.setHealth(std::min(newHealth, logic::DEFAULT_MAX_HEALTH));
        }
    }
}