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
        std::vector<CombatState> active_combats;

        CombatLogic logic;

        constexpr static int BASE_DAMAGE = 10;
        constexpr static int BASE_HEAL = 5;
    public:
        CombatHandler();

        /**
         *
         * @param attacker that will enter Combat
         * @param defender that will enter Combat
         */
        void enterCombat(const Character &attacker, const Character &defender);

        /**
         * The arguments are attacker and defender for name consistency sake,
         * it doesn't matter in which order the Characters are passed in
         * @param attacker: First character that is part of the Combat
         * @param defender: Second character that is part of the Combat
         */
        void exitCombat(Character &attacker, Character &defender);

        /**
         *
         * @param attacker is the person dealing damage
         * @param defender is the perosn being dealt the damage
         */
        void attack(Character &attacker, Character &defender);

        /**
         *
         * @param healer that will use their ability to heal someone
         * @param target is the person being healed
         */
        void heal(Character &healer, Character &target);
    };
}



#endif //COMBATHANDLER_H
