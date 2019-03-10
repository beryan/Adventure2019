//
// Created by jnhkm on 2019-03-10.
//

#include "CombatLogic.h"

using logic::CombatLogic;

namespace logic {
    CombatLogic::CombatLogic() {}

    bool CombatLogic::canAttackTarget(Character attacker, Characted defender) {
        bool result = false;

        // Check flags, magic, state, whatever
        // Set result to true if they are able to be attacked

        return result;
    }

    bool CombatLogic::canFlee(Character character) {
        bool result = false;

        // Check if the Character can flee in its current state

        return result;
    }
}