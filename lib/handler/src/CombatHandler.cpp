//
// Created by jnhkm on 2019-03-10.
//

#include "CombatHandler.h"
#include <iostream>
#include <sstream>

using handler::CombatHandler;

namespace handler {
    CombatHandler::CombatHandler(AccountHandler &accountHandler, WorldHandler &worldHandler) :
        combatInstances{},
        accountHandler(accountHandler),
        worldHandler(worldHandler),
        RNG(std::random_device{}()){}


    void CombatHandler::enterCombat(const Character &attacker, const Character &defender) {
        combatInstances.emplace_back(CombatInstance{attacker.getId(), defender.getId()});
    }


    void CombatHandler::exitCombat(const Character &attacker, const Character &defender) {
        auto it = std::find(
                combatInstances.begin(),
                combatInstances.end(),
                CombatInstance{attacker.getId(), defender.getId()});

        if (it != combatInstances.end()) {
            combatInstances.erase(it);
        }
    }


    void CombatHandler::exitCombat(const Character &character) {
        auto characterId = character.getId();
        auto combat_it = std::find_if(
                combatInstances.begin(),
                combatInstances.end(),
                [&characterId](const auto &combatState) {
                    return (combatState.attackerID == characterId || combatState.defenderID == characterId);
                }
        );

        combatInstances.erase(combat_it);
    }


    void CombatHandler::inflictDamage(Character &defender) {
        std::uniform_int_distribution damageRoll(BASE_MIN_DAMAGE, BASE_MAX_DAMAGE);
        std::bernoulli_distribution criticalProc(BASE_CRITICAL_CHANCE);

        auto damage = damageRoll(this->RNG);

        if (criticalProc(this->RNG)) {
            damage = static_cast<int>(static_cast<float>(damage) * BASE_CRITICAL_DAMAGE_MULTIPLIER);
        }

        int newHealth = defender.getHealth() - damage;

        defender.setHealth(std::max(newHealth, 0));
    }

    std::string CombatHandler::attack(const Connection &client, const std::string &targetName) {
        std::ostringstream message;

        auto roomId = this->accountHandler.getRoomIdByClient(client);
        auto &room = this->worldHandler.findRoom(roomId);

        try {
            // try getting npc with targetName, throws exception if not found.
            auto &npc = room.getNpcByKeyword(targetName);

            auto player = this->accountHandler.getPlayerByClient(client);

            bool playerInCombat = this->isInCombat(*player);
            bool npcInCombat = this->isInCombat(npc);
            bool npcInCombatWithPlayer = this->areInCombat(*player, npc);

            if (npcInCombat && !npcInCombatWithPlayer) {
                message << targetName<< " is already engaged in combat with someone else!\n";
                return message.str();
            }

            if (playerInCombat && !npcInCombatWithPlayer) {
                message << "You are already engaged in combat with someone else!\n";
                return message.str();
            }

            if (!npcInCombat) {
                this->enterCombat(*player, npc);
            }

            auto npcHpBefore = npc.getHealth();
            this->inflictDamage(npc);
            auto npcHpAfter = npc.getHealth();

            message << "\n"
                    << "You inflict " << (npcHpBefore - npcHpAfter)
                    << " HP worth of damage to " << npc.getShortDescription()
                    << " (" << npcHpAfter << " HP remaining)\n";


            if (npc.getHealth() == 0) {
                message << "You won the battle!\n";
                this->exitCombat(*player, npc);
                player->setHealth(Character::STARTING_HEALTH);
                npc.setHealth(Character::STARTING_HEALTH);

                return message.str();
            }

            auto playerHpBefore = player->getHealth();
            this->inflictDamage(*player);
            auto playerHpAfter = player->getHealth();

            message << npc.getShortDescription() << " inflicts "
                    << (playerHpBefore - playerHpAfter) << " HP worth of damage on you ("
                    << playerHpAfter << " HP remaining)\n";

            if (player->getHealth() == 0) {
                message << "You lost the battle.\n";
                this->exitCombat(*player, npc);
                player->setHealth(Character::STARTING_HEALTH);
                npc.setHealth(Character::STARTING_HEALTH);
            }

            auto combat_it = std::find_if(
                    combatInstances.begin(),
                    combatInstances.end(),
                    [&player](const auto &combatState) {
                        return (combatState.attackerID == player->getId());
                    }
            );

            combat_it->endRound();

            return message.str();

        } catch (const std::runtime_error &e) {
            message << "There is no one here with the name of " << targetName << "\n";
            return message.str();
        }
    }


    std::string CombatHandler::flee(const Connection &client) {
        std::ostringstream message;

        auto player = this->accountHandler.getPlayerByClient(client);
        auto playerId = this->accountHandler.getPlayerIdByClient(client);

        if (!this->isInCombat(*player)) {
            message << "You are in no danger to flee from.\n";
            return message.str();
        }

        auto roomId = this->accountHandler.getRoomIdByClient(client);
        auto doors = this->worldHandler.findRoom(roomId).getDoors();

        // Corner case where room has no doors
        if (doors.empty()) {
            // Player has halved chance of fleeing successfully
            std::bernoulli_distribution fleeChance{BASE_FLEE_CHANCE / 2};

            if (fleeChance(this->RNG)) {
                this->exitCombat(*player);
                message << "You successfully flee from combat.\n";

            } else {
                auto opponentId = this->getOpponentId(*player);
                auto &npc = this->worldHandler.findRoom(roomId).getNpcById(opponentId);

                auto playerHpBefore = player->getHealth();
                this->inflictDamage(*player);
                auto playerHpAfter = player->getHealth();

                message << "You attempt to flee from combat, but fail. ("
                            << (BASE_FLEE_CHANCE / 2 * 100) << "% chance of success)\n";

                message << npc.getShortDescription() << " inflicts "
                            << (playerHpBefore - playerHpAfter) << " HP worth of damage on you ("
                            << playerHpAfter << " HP remaining)\n";

                if (player->getHealth() == 0) {
                    message << "You lost the battle.\n";
                    this->exitCombat(*player, npc);
                    player->setHealth(Character::STARTING_HEALTH);
                    npc.setHealth(Character::STARTING_HEALTH);
                }
            }

            return message.str();
        }

        // Player has chance of fleeing to a random direction successfully
        std::bernoulli_distribution fleeChance(BASE_FLEE_CHANCE * doors.size());

        if (fleeChance(this->RNG)) {
            std::uniform_int_distribution<unsigned long> pickDoor(0, (doors.size() - 1));

            auto door = doors.at(pickDoor(this->RNG));
            auto direction = door.dir;
            auto destinationId = door.leadsTo;

            this->worldHandler.movePlayer(playerId, roomId, destinationId);
            this->accountHandler.setRoomIdByClient(client, destinationId);

            this->exitCombat(*player);
            message << "You successfully flee to the " << direction << ".\n";

        } else {
            auto opponentId = this->getOpponentId(*player);
            auto &npc = this->worldHandler.findRoom(roomId).getNpcById(opponentId);

            auto playerHpBefore = player->getHealth();
            this->inflictDamage(*player);
            auto playerHpAfter = player->getHealth();

            message << "You attempt to flee, but fail. ("
                        << (BASE_FLEE_CHANCE * doors.size() * 100) << "% chance of success)\n";

            message << npc.getShortDescription() << " inflicts "
                        << (playerHpBefore - playerHpAfter) << " HP worth of damage on you ("
                        << playerHpAfter << " HP remaining)\n";

            if (player->getHealth() == 0) {
                message << "You lost the battle.\n";
                this->exitCombat(*player, npc);
                player->setHealth(Character::STARTING_HEALTH);
                npc.setHealth(Character::STARTING_HEALTH);
            }
        }

        return message.str();
    }


    bool CombatHandler::areInCombat(const Character &attacker, const Character &defender) {
        auto attackerId = attacker.getId();
        auto defenderId = defender.getId();

        auto combat_it = std::find(
                this->combatInstances.begin(),
                this->combatInstances.end(),
                CombatInstance{attackerId, defenderId}
        );

        return combat_it != this->combatInstances.end();
    }


    bool CombatHandler::isInCombat(const Character &character) {
        auto characterId = character.getId();

        auto combat_it = std::find_if(
            this->combatInstances.begin(),
            this->combatInstances.end(),
            [&characterId](const auto &combatState) {
                return (combatState.attackerID == characterId || combatState.defenderID == characterId);
            }
        );

        return combat_it != this->combatInstances.end();
    };

    model::ID CombatHandler::getOpponentId(const Character &character) {
        auto characterId = character.getId();

        auto combat_it = std::find_if(
                this->combatInstances.begin(),
                this->combatInstances.end(),
                [&characterId](const auto &combatInstance) {
                    return (combatInstance.attackerID == characterId || combatInstance.defenderID == characterId);
                }
        );

        if (combat_it == this->combatInstances.end()) {
            return 0;
        }

        if (combat_it->attackerID == characterId) {
            return combat_it->defenderID;

        } else {
            return combat_it->attackerID;
        }
    }


    void CombatHandler::processCycle(std::deque<Message> &messages) {
        std::vector<Player*> defeatedPlayers;

        for (auto &combatInstance : this->combatInstances) {
            if (combatInstance.roundCyclesRemaining > 0) {
                combatInstance.decrement();

            } else {
                auto client = this->accountHandler.getClientByPlayerId(combatInstance.attackerID);
                auto player = this->accountHandler.getPlayerByClient(client);
                auto roomId = this->accountHandler.getRoomIdByClient(client);
                auto &npc = this->worldHandler.findRoom(roomId).getNpcById(combatInstance.defenderID);

                auto playerHpBefore = player->getHealth();
                this->inflictDamage(*player);
                auto playerHpAfter = player->getHealth();

                std::ostringstream message;
                message << npc.getShortDescription() << " inflicts "
                        << (playerHpBefore - playerHpAfter) << " HP worth of damage on you ("
                        << playerHpAfter << " HP remaining)\n";

                if (player->getHealth() == 0) {
                    message << "You lost the battle.\n";
                    defeatedPlayers.push_back(player);
                    player->setHealth(Character::STARTING_HEALTH);
                    npc.setHealth(Character::STARTING_HEALTH);
                }

                messages.push_back({client, message.str()});
                combatInstance.endRound();
            }
        }

        for (const auto &defeatedPlayer : defeatedPlayers) {
            this->exitCombat(*defeatedPlayer);
        }
    }
}