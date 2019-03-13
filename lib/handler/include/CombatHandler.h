//
// Created by jnhkm on 2019-03-10.
//

#ifndef COMBATHANDLER_H
#define COMBATHANDLER_H

#include "CombatLogic.h"
#include "Character.h"

using logic::CombatLogic;
using logic::CombatState;
using model::Character;

namespace action {
    class CombatHandler {
    private:
        std::vector<CombatState> active_combats = {};

        CombatLogic logic = {active_combats};

        constexpr static int BASE_DAMAGE = 10;
        constexpr static int BASE_HEAL = 5;
    public:
        CombatHandler();

        void enterCombat(const Character &attacker, const Character &defender);

        void exitCombat(Character &attacker, Character &defender);

        void attack(Character &attacker, Character &defender);

        void heal(Character &healer, Character &target);
    };
}



#endif //COMBATHANDLER_H
