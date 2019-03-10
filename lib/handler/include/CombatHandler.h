//
// Created by jnhkm on 2019-03-10.
//

#ifndef COMBATHANDLER_H
#define COMBATHANDLER_H

#include "CombatLogic.h"
#include "Character.h"
#include "CharacterHandler.h"

using action::CharacterHandler;
using logic::CombatLogic;
using model::Character;

namespace action {
    class CombatHandler {
    private:
        CombatLogic logic = {};
        CharacterHandler handler = {};
        int baseDamage = 1;
    public:
        CombatHandler();

        void attack(Character &attacker, Character &defender);
    };
}



#endif //COMBATHANDLER_H
