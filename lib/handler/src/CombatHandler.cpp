//
// Created by jnhkm on 2019-03-10.
//

#include "CombatHandler.h"

using action::CombatHandler;
using action::CharacterHandler;

namespace action {
    CombatHandler::CombatHandler() {}

    void CombatHandler::attack(Character &attacker, Character &defender) {
        handler.reduceHealth(defender, baseDamage);
    }
}