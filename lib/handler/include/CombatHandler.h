//
// Created by jnhkm on 2019-03-10.
//

#ifndef COMBATHANDLER_H
#define COMBATHANDLER_H

#include "CombatLogic.h"
#include "Character.h"

using logic::CombatLogic;
using model::Character;

namespace action {
    class CombatHandler {
    private:
        CombatLogic logic = {};
        constexpr static int BASE_DAMAGE = 10;
        constexpr static int BASE_HEAL = 5;
    public:
        CombatHandler();

        void attack(Character &attacker, Character &defender);

        void heal(Character &healer, Character &target);
    };
}



#endif //COMBATHANDLER_H
