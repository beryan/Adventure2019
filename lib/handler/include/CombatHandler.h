//
// Created by jnhkm on 2019-03-10.
//

#ifndef COMBATHANDLER_H
#define COMBATHANDLER_H

#include <random>
#include "CombatLogic.h"
#include "Character.h"

using logic::CombatLogic;
using model::Character;

namespace handler {
    class CombatHandler {
    private:
        std::vector<CombatState> active_combat;

        CombatLogic logic;

        std::mt19937 RNG;

        constexpr static int BASE_MIN_DAMAGE = 10;
        constexpr static int BASE_MAX_DAMAGE = 20;
        constexpr static int BASE_HEAL = 5;

    public:
        CombatHandler();

        constexpr static float BASE_FLEE_CHANCE = 0.2f;
        constexpr static float BASE_CRITICAL_CHANCE = 0.07f;

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
        void exitCombat(const Character &attacker, const Character &defender);

        /**
         * Erases a combat state involving character
         * @param character: character that is in combat
         */
        void exitCombat(const Character &character);

        /**
         *
         * @param attacker is the person dealing damage
         * @param defender is the perosn being dealt the damage
         */
        void attack(const Character &attacker, Character &defender);

        /**
         *
         * @param healer that will use their ability to heal someone
         * @param target is the person being healed
         */
        void heal(const Character &healer, Character &target);

        /**
         *
         * @param attacker that is attacking the target
         * @param defender that is taking the damage
         * @return True if there is an active combat state with both characters, or False otherwise
         */
        bool areInCombat(const Character &attacker, const Character &defender);

        /**
         *
         * @param character to be evaluated
         * @return True if character is in an active combat state, or False otherwise
         */
        bool isInCombat(const Character &character);

        model::ID getOpponentId(const Character &character);

        std::mt19937& generateRandom();
    };
}



#endif //COMBATHANDLER_H
