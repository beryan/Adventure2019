//
// Created by jnhkm on 2019-03-10.
//

#include "CombatHandler.h"

using action::CombatHandler;

namespace action {
    CombatHandler::CombatHandler() {}

    void CombatHandler::attack(Character &attacker, Character &defender) {
        if (logic.canAttackTarget(attacker, defender)) {
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