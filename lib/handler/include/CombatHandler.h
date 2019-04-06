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
         * @param attacker: character that will enter Combat with defender
         * @param defender: NPC that will enter Combat with attacker
         */
        void enterCombat(const Character &attacker, const NPC &defender);

        /**
         * @return true if miss occurs, otherwise false
         */
        bool rollMiss();

        /**
         * @param modifier: floating point value of increase or decrease in probability of dodging
         * @return true if dodge occurs, otherwise false
         */
        bool rollDodge(const float &modifier = 0);

        /**
         * @param modifier: integer value of increase or decrease in minimum and maximum damage
         * @return integer value of the damage between the defined range
         */
        int rollDamage(const int &modifier = 0);

        /**
         * @param modifier: floating point value of increase or decrease in probability of inflicting a critical strike
         * @return true if a critical strike occurs, false otherwise
         */
        bool rollCritical(const float &modifier = 0);

        /**
         * Methods for applying an attack result to the npc or player's health.
         * Performs chance rolls for misses, dodges, damage, and critical strikes.
         * @param npc: NPC that is being attacked
         * @return string containing the attack result
         */
        std::string inflictDamage(NPC &npc);

        /**
         * @param player: Player that is being attacked
         * @return string containing the attack result
         */
        std::string inflictDamage(Player &player);

        /**
         * Performs defined operations when a player has won a battle.
         * @param player: Player in combat that has won the battle
         * @param npc: NPC in combat that has lost the battle
         * @return string containing the winning message
         */
        std::string winEvent(Player &player, NPC &npc);

        /**
         * Performs defined operations when a player has lost a battle
         * @param player: Player in combat that has lost the battle
         * @param npc: NPC in combat that has won the battle
         * @return string containing the losing message
         */
        std::string loseEvent(Player &player, NPC &npc);

    public:
        constexpr static int BASE_MIN_DAMAGE = 10;
        constexpr static int BASE_MAX_DAMAGE = 20;


        CombatHandler(AccountHandler &accountHandler, WorldHandler &worldHandler);

        /**
         * Causes the client's player to enter combat with an NPC if not already in combat.
         * Executes an attack on the targetted NPC, followed by a retaliation, then ends the round.
         * Also includes a win or loss message if either is achieved.
         * @param client: Connection that is performing the action
         * @param targetName: the keyword to specify the target in the room
         * @return string containing the attack result
         */
        std::string attack(const Connection &client, const std::string &targetName);

        /**
         * Causes the client's player to attempt to escape combat by fleeing to a random connected room.
         * The player can escape combat without changing rooms if there are no connected rooms.
         * @param client: Connection that is performing the action
         * @return string containing the flee result
         */
        std::string flee(const Connection &client);

        /**
         * Used for checking if a character is currently in combat
         * @param id: player ID or NPC unique ID to be evaluated
         * @return true if character is in an active combat state, otherwise false
         */
        bool isInCombat(const model::ID &id);

        /**
         * Used for checking if two characters are currently in combat with each other
         * @param combatantId1: character ID that is in combat with combatantId2
         * @param combatantId2: character ID that is in combat with combatantId1
         * @return true if there is an active combat state with both characters, otherwise false
         */
        bool areInCombat(const model::ID &combatantId1, const model::ID &combatantId2);

        /**
         * Removes a combat instance in which two characters are involved in
         * @param combatantId1: character ID that is in combat with combatantId2
         * @param combatantId2: character ID that is in combat with combatantId1
         */
        void exitCombat(const model::ID &combatantId1, const model::ID &combatantId2);

        /**
         * Removes a combat instance involving character
         * @param character: character that is in combat
         */
        void exitCombat(const model::ID &id);

        /**
         * Used for getting the ID of the opponent of the character in a combat instance
         * @param character: Character to be evaluated
         * @return ID of the opposing NPC or Player
         */
        model::ID getOpponentId(const model::ID &id);

        /**
         * Replaces the Player in a combat instance with a decoy NPC
         * @param player: Player to be replaced by a decoy
         */
        void replaceWithDecoy(const Player &player);


        /**
         * Used for performing the correct actions if a client disconnects from the game
         * while they are in combat.
         * @param client: Connection that has disconnected from the game
         */
        void handleLogout(const Connection &client);

        /**
         * Decrements the cycle counter of the round in each combat instance and returns
         * combat-related messages when appropriate.
         * @param messages: A message deque in which messages will be added to
         */
        void processCycle(std::deque<Message> &messages);

    };
}



#endif //COMBATHANDLER_H
