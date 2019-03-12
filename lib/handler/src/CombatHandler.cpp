//
// Created by jnhkm on 2019-03-10.
//

#include "CombatHandler.h"

using action::CombatHandler;

namespace action {
    CombatHandler::CombatHandler() {}

    void CombatHandler::attack(Character &attacker, Character &defender) {
        if (logic.canAttackTarget(attacker, defender)) {
            defender.getHealth() - BASE_DAMAGE >= logic::DEFAULT_MIN_HEALTH ?
                defender.setHealth(defender.getHealth() - BASE_DAMAGE) :
                defender.setHealth(0);
        }
    }

    void CombatHandler::heal(Character &healer, Character &target) {
        if (logic.canHealTarget(healer, target)) {
            target.getHealth() + BASE_HEAL <= logic::DEFAULT_MAX_HEALTH ?
                target.setHealth(target.getHealth() + BASE_HEAL) :
                target.setHealth(logic::DEFAULT_MAX_HEALTH);
        }
    }
}