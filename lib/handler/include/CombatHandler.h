//
// Created by jnhkm on 2019-03-10.
//

#ifndef COMBATHANDLER_H
#define COMBATHANDLER_H

#include <random>

#include "Server.h"
#include "CombatInstance.h"
#include "Character.h"
#include "AccountHandler.h"
#include "WorldHandler.h"

using model::CombatInstance;
using model::Character;
using networking::Message;

namespace handler {
    class CombatHandler {
    private:
        std::vector<CombatInstance> combatInstances;
        AccountHandler& accountHandler;
        WorldHandler& worldHandler;

        std::mt19937 RNG;

        constexpr static int BASE_MIN_DAMAGE = 10;
        constexpr static int BASE_MAX_DAMAGE = 20;
        constexpr static float BASE_FLEE_CHANCE = 0.2f;

        constexpr static float BASE_CRITICAL_CHANCE = 0.07f;
        constexpr static float BASE_CRITICAL_DAMAGE_MULTIPLIER = 2.5f;

        /**
         *
         * @param defender is the perosn being dealt the damage
         */
        void inflictDamage(Character &defender);

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

    public:
        CombatHandler(AccountHandler &accountHandler, WorldHandler &worldHandler);

        std::string attack(const Connection &client, const std::string &targetName);
        std::string flee(const Connection &client);

        /**
         * @param character to be evaluated
         * @return True if character is in an active combat state, or False otherwise
         */
        bool isInCombat(const Character &character);

        /**
         * @param attacker that is attacking the target
         * @param defender that is taking the damage
         * @return True if there is an active combat state with both characters, or False otherwise
         */
        bool areInCombat(const Character &attacker, const Character &defender);


        model::ID getOpponentId(const Character &character);


        void processCycle(std::deque<Message> &messages);

    };
}



#endif //COMBATHANDLER_H
