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


    void
    CombatHandler::enterCombat(const Character &attacker, const Character &defender) {
        combatInstances.emplace_back(CombatInstance{attacker.getId(), defender.getId()});
    }


    void
    CombatHandler::exitCombat(const Character &character1, const Character &character2) {
        auto it = std::find(
                combatInstances.begin(),
                combatInstances.end(),
                CombatInstance{character1.getId(), character2.getId()});

        if (it != combatInstances.end()) {
            combatInstances.erase(it);
        }
    }


    void
    CombatHandler::exitCombat(const Character &character) {
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


    bool
    CombatHandler::rollMiss() {
        std::bernoulli_distribution missProc(BASE_MISS_CHANCE);

        return missProc(this->RNG);
    }


    bool
    CombatHandler::rollDodge(const float &modifier) {
        std::bernoulli_distribution dodgeProc(BASE_DODGE_CHANCE + modifier);

        return dodgeProc(this->RNG);
    }


    int
    CombatHandler::rollDamage(const int &modifier) {
        std::uniform_int_distribution damageRoll(BASE_MIN_DAMAGE + modifier, BASE_MAX_DAMAGE + modifier);

        return damageRoll(this->RNG);
    }


    bool
    CombatHandler::rollCritical(const float &modifier) {
        std::bernoulli_distribution criticalProc(BASE_CRITICAL_CHANCE + modifier);

        return criticalProc(this->RNG);
    }


    std::string
    CombatHandler::inflictDamage(NPC &npc) {
        std::ostringstream message;
        auto playerId = this->getOpponentId(npc);
        auto client = this->accountHandler.getClientByPlayerId(playerId);
        auto player = this->accountHandler.getPlayerByClient(client);

        if (this->rollMiss()) {
            message << "You miss your attack on " << npc.getShortDescription() << "!\n";
            return message.str();
        }

        auto dodgeValue = player->getEquipment().getDodgeValue();
        if (this->rollDodge(dodgeValue)) {
            message << npc.getShortDescription() << " dodges your attack!\n";
            return message.str();
        }

        auto offenceValue = player->getEquipment().getOffenceValue();
        auto damage = this->rollDamage(offenceValue);

        auto criticalValue = player->getEquipment().getCriticalValue();
        if (this->rollCritical(criticalValue)) {
            damage = static_cast<int>(static_cast<float>(damage) * BASE_CRITICAL_DAMAGE_MULTIPLIER);
            message << "You strike a critical hit, inflicting ";
        } else {
            message << "You inflict ";
        }


        int oldHealth = npc.getHealth();
        int newHealth = std::max(npc.getHealth() - damage, 0);
        npc.setHealth(newHealth);

        message << (oldHealth - newHealth) << " HP worth of damage to "
                << npc.getShortDescription() << " (" << npc.getHealth() << " HP remaining)\n";

        return message.str();
    }


    std::string
    CombatHandler::inflictDamage(Player &player) {
        std::ostringstream message;
        auto client = this->accountHandler.getClientByPlayerId(player.getId());
        auto roomId = this->accountHandler.getRoomIdByClient(client);
        auto npcId = this->getOpponentId(player);
        auto npc = this->worldHandler.findRoom(roomId).getNpcById(npcId);
        auto npcName = npc.getShortDescription();

        if (this->rollMiss()) {
            message << npcName << " misses their attack on you!\n";
            return message.str();
        }

        if (this->rollDodge()) {
            message << "You dodge " << npcName << "'s attack!\n";
            return message.str();
        }

        auto damage = this->rollDamage();
        damage -= player.getEquipment().getDefenceValue();

        if (this->rollCritical()) {
            damage = static_cast<int>(static_cast<float>(damage) * BASE_CRITICAL_DAMAGE_MULTIPLIER);
            message << npcName << " strikes a critical hit, inflicting ";
        } else {
            message << npcName << " inflicts ";
        }

        int oldHealth = player.getHealth();
        int newHealth = std::max(player.getHealth() - damage, 0);
        player.setHealth(newHealth);

        message << (oldHealth - newHealth) << " HP worth of damage on you ("
                << player.getHealth() << " HP remaining)\n";

        return message.str();
    }


    std::string
    CombatHandler::attack(const Connection &client, const std::string &targetName) {
        std::ostringstream message;

        if (targetName.empty()) {
            message << "You need to specify the name of the person (NPC) to attack.\n";
            return message.str();
        }

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
                message << targetName << " is already engaged in combat with someone else!\n";
                return message.str();
            }

            if (playerInCombat && !npcInCombatWithPlayer) {
                message << "You are already engaged in combat with someone else!\n";
                return message.str();
            }

            if (!npcInCombat) {
                this->enterCombat(*player, npc);
                message << "\nYou engage in combat with " << npc.getShortDescription() << "!\n";
            }

            message << "\n" << this->inflictDamage(npc);

            if (npc.getHealth() == 0) {
                message << "You won the battle!\n";
                this->exitCombat(*player, npc);
                player->setHealth(Character::STARTING_HEALTH);
                npc.setHealth(Character::STARTING_HEALTH);

                return message.str();
            }

            message << this->inflictDamage(*player);

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
            message << "There is no one here (NPC) with the name " << targetName << "\n";
            return message.str();
        }
    }


    std::string
    CombatHandler::flee(const Connection &client) {
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


                message << "You attempt to flee, but fail. ("
                            << (BASE_FLEE_CHANCE / 2 * 100) << "% chance of success)\n";

                message << this->inflictDamage(*player);

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


    bool
    CombatHandler::areInCombat(const Character &attacker, const Character &defender) {
        auto attackerId = attacker.getId();
        auto defenderId = defender.getId();

        auto combat_it = std::find(
                this->combatInstances.begin(),
                this->combatInstances.end(),
                CombatInstance{attackerId, defenderId}
        );

        return combat_it != this->combatInstances.end();
    }


    bool
    CombatHandler::isInCombat(const Character &character) {
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


    model::ID
    CombatHandler::getOpponentId(const Character &character) {
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


    void
    CombatHandler::handleLogout(const Connection &client) {
        auto player = this->accountHandler.getPlayerByClient(client);
        auto roomId = this->accountHandler.getRoomIdByClient(client);
        auto npcId = this->getOpponentId(*player);
        auto &npc = this->worldHandler.findRoom(roomId).getNpcById(npcId);

        this->exitCombat(*player);
        npc.setHealth(Character::STARTING_HEALTH);
    }


    void
    CombatHandler::processCycle(std::deque<Message> &messages) {
        std::vector<Player*> defeatedPlayers;

        for (auto &combatInstance : this->combatInstances) {
            if (combatInstance.roundCyclesRemaining > 0) {
                combatInstance.decrement();

            } else {
                auto client = this->accountHandler.getClientByPlayerId(combatInstance.attackerID);
                auto player = this->accountHandler.getPlayerByClient(client);
                auto roomId = this->accountHandler.getRoomIdByClient(client);
                auto &npc = this->worldHandler.findRoom(roomId).getNpcById(combatInstance.defenderID);

                std::ostringstream message;
                message << "\n" << this->inflictDamage(*player);

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