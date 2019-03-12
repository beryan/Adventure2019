//
// Created by jnhkm on 2019-03-10.
//

#include "CombatHandler.h"

using action::CombatHandler;

namespace action {
    CombatHandler::CombatHandler() {}

    void CombatHandler::attack(Character &attacker, Character &defender) {
        if (logic.canAttackTarget(attacker, defender)) {
            int change = defender.getHealth() - BASE_DAMAGE;

            change >= logic::DEFAULT_MIN_HEALTH ?
                defender.setHealth(change) :
                defender.setHealth(logic::DEFAULT_MIN_HEALTH);
        }
    }

    void CombatHandler::heal(Character &healer, Character &target) {
        if (logic.canHealTarget(healer, target)) {
            int change = target.getHealth() + BASE_HEAL;

            change <= logic::DEFAULT_MAX_HEALTH ?
                target.setHealth(change) :
                target.setHealth(logic::DEFAULT_MAX_HEALTH);
        }
    }
}