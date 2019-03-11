//
// Created by jnhkm on 2019-03-10.
//

#include "CombatHandler.h"

using action::CombatHandler;

namespace action {
    CombatHandler::CombatHandler() {}

    void CombatHandler::attack(Character &attacker, Character &defender) {
        if (logic.canAttackTarget(attacker, defender)) {
            defender.setHealth(defender.getHealth() - baseDamage);
        }
        else {
            defender.setHealth(logic::DEFAULT_MIN_HEALTH);
        }
    }

    void CombatHandler::heal(Character &target) {
        if (logic.canHealTarget(target)) {
            target.setHealth(target.getHealth() + baseHeal);
        }
        else {
            target.setHealth(logic::DEFAULT_MAX_HEALTH);
        }
    }
}