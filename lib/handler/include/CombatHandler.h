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

        constexpr static float BASE_FLEE_CHANCE = 0.2f;
        constexpr static float BASE_MISS_CHANCE = 0.05f;
        constexpr static float BASE_DODGE_CHANCE = 0.1f;
        constexpr static float BASE_CRITICAL_CHANCE = 0.07f;
        constexpr static float BASE_CRITICAL_DAMAGE_MULTIPLIER = 2.5f;

        /**
         *  Returns true if miss occurs
         */
        bool
        rollMiss();

        /**
         *  Returns true if dodge occurs
         */
        bool
        rollDodge(const float &modifier = 0);

        /**
         *  Returns a damage value between a defined range
         */
        int
        rollDamage(const int &modifier = 0);

        /**
         *  Returns true if a critical strike occurs
         */
        bool
        rollCritical(const float &modifier = 0);

        /**
         *  Methods for applying an attack result to the npc or player's health.
         *  Performs chance rolls for misses, dodges, damage, and critical strikes.
         *  Returns a message based on the result.
         * @param npc: NPC that is being attacked
         */
        std::string
        inflictDamage(NPC &npc);

        /**
         * @param player: player that is being attacked
         */
        std::string
        inflictDamage(Player &player);

        /**
         * @param attacker: character that will enter Combat with defender
         * @param defender character that will enter Combat with attacker
         */
        void
        enterCombat(const Character &attacker, const Character &defender);

        /**
         * Disengages two characters from combat where they are both involved in
         * @param character1: character that is in combat with character2
         * @param character2: character that is in combat with character1
         */
        void
        exitCombat(const Character &character1, const Character &character2);

        /**
         * Erases a combat state involving character
         * @param character: character that is in combat
         */
        void
        exitCombat(const Character &character);

    public:
        constexpr static int BASE_MIN_DAMAGE = 10;
        constexpr static int BASE_MAX_DAMAGE = 20;


        CombatHandler(AccountHandler &accountHandler, WorldHandler &worldHandler);

        /**
         * Causes the client's player to enter combat with an NPC if not already in combat.
         * Executes an attack on the targetted NPC, followed by a retaliation, then ends the round.
         * Also includes a win or loss message if either is achieved.
         * @param client that is performing the action
         * @param targetName: the keyword to specify the target in the room
         * @return string containing the attack result
         */
        std::string
        attack(const Connection &client, const std::string &targetName);

        /**
         * Causes the client's player to attempt to escape combat by fleeing to a random connected room.
         * The player can escape combat without changing rooms if there are no connected rooms.
         * @param client that is performing the action
         * @return string containing the flee result
         */
        std::string
        flee(const Connection &client);

        /**
         * Used for checking if a character is currently in combat
         * @param character to be evaluated
         * @return true if character is in an active combat state, otherwise false
         */
        bool
        isInCombat(const Character &character);

        /**
         * @param attacker: character that is in combat with the defender
         * @param defender: character that is in combat with the attacker
         * @return true if there is an active combat state with both characters, otherwise false
         */
        bool
        areInCombat(const Character &attacker, const Character &defender);


        /**
         * Used for getting the ID of the opponent of the character in a combat instance
         * @param character to be evaluated
         * @return ID of the opposing NPC or Player
         */
        model::ID
        getOpponentId(const Character &character);


        /**
         * Replaces the character in a combat instance with a dummy NPC
         * @param character to be replaced
         */
        void
        replaceWithDummy(const Player &player);


        /**
         * Used for performing the correct actions if a client disconnects from the game
         * while they are in combat.
         * @param client that has disconnected from the game
         */
        void
        handleLogout(const Connection &client);

        /**
         * Decrements the cycle counter of the round in each combat instance and returns
         * combat-related messages when appropriate.
         * @param messages deque from the game class
         */
        void
        processCycle(std::deque<Message> &messages);

    };
}



#endif //COMBATHANDLER_H
