//
// Created by jnhkm on 2019-03-10.
//

#include "CombatHandler.h"
#include <iostream>
#include <sstream>
#include <boost/algorithm/string.hpp>

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

        if (client.id != AccountHandler::INVALID_ID) {
            auto &player = this->accountHandler.getPlayerByClient(client);

            if (this->rollMiss()) {
                message << "You miss your attack on " << npc.getShortDescription() << "!\n";
                return message.str();
            }

            auto dodgeValue = player.getMutableEquipment().getDodgeValue();
            if (this->rollDodge(dodgeValue)) {
                message << npc.getShortDescription() << " dodges your attack!\n";
                return message.str();
            }

            auto offenceValue = player.getMutableEquipment().getOffenceValue();
            auto damage = this->rollDamage(offenceValue);

            auto criticalValue = player.getMutableEquipment().getCriticalValue();
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

        } else {
            // Case in which the NPC being damaged is a decoy
            auto damage = this->rollDamage();

            int newHealth = std::max(npc.getHealth() - damage, 0);
            npc.setHealth(newHealth);

            return "";
        }
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
        damage -= player.getMutableEquipment().getDefenceValue();

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
    CombatHandler::winEvent(Player &player, NPC &npc) {
        std::ostringstream message;
        message << "You won the battle!\n";
        this->exitCombat(player, npc);

        // Remove NPC from room
        auto &room = this->worldHandler.findRoom(player.getCurrRoomID());
        auto &npcs = room.getNpcs();
        auto npc_it = std::find_if(
                npcs.begin(),
                npcs.end(),
                [&npc](const auto &roomNpc) {
                    return (roomNpc.getId() == npc.getId());
                });
        npcs.erase(npc_it);

        return message.str();
    }


    std::string
    CombatHandler::loseEvent(Player &player, NPC& npc) {
        std::ostringstream message;
        message << "You have fallen in battle.\n"
                << "You reawaken at where you began.\n";
        this->exitCombat(player, npc);

        npc.setHealth(Character::STARTING_HEALTH);
        player.setHealth(Character::STARTING_HEALTH);
        auto &room = this->worldHandler.findRoom(player.getCurrRoomID());
        room.removePlayerFromRoom(player.getId());
        player.setCurrRoomID(Player::STARTING_LOCATION);

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

            auto &player = this->accountHandler.getPlayerByClient(client);

            bool playerInCombat = this->isInCombat(player);
            bool npcInCombat = this->isInCombat(npc);
            bool npcInCombatWithPlayer = this->areInCombat(player, npc);

            if (npcInCombat && !npcInCombatWithPlayer) {
                message << targetName << " is already engaged in combat with someone else!\n";
                return message.str();
            }

            if (playerInCombat && !npcInCombatWithPlayer) {
                message << "You are already engaged in combat with someone else!\n";
                return message.str();
            }

            if (!npcInCombat) {
                this->enterCombat(player, npc);
                message << "\nYou engage in combat with " << npc.getShortDescription() << "!\n";
            }

            message << "\n" << this->inflictDamage(npc);

            if (npc.getHealth() == 0) {
                message << this->winEvent(player, npc);
            }

            message << this->inflictDamage(player);

            if (player.getHealth() == 0) {
                message << this->loseEvent(player, npc);
            }

            auto combat_it = std::find_if(
                    combatInstances.begin(),
                    combatInstances.end(),
                    [&player](const auto &combatState) {
                        return (combatState.attackerID == player.getId());
                    }
            );

            combat_it->endRound();

            return message.str();

        } catch (const std::runtime_error &e) {
            message << "Invalid keyword.\n";
            return message.str();
        }
    }


    std::string
    CombatHandler::flee(const Connection &client) {
        std::ostringstream message;

        auto &player = this->accountHandler.getPlayerByClient(client);
        auto playerId = this->accountHandler.getPlayerIdByClient(client);

        if (!this->isInCombat(player)) {
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
                this->exitCombat(player);
                message << "You successfully flee from combat.\n";

            } else {
                auto opponentId = this->getOpponentId(player);
                auto &npc = this->worldHandler.findRoom(roomId).getNpcById(opponentId);

                message << "\n"
                        << "You attempt to flee, but fail. ("
                        << (BASE_FLEE_CHANCE / 2 * 100) << "% chance of success)\n";

                message << this->inflictDamage(player);

                auto combat_it = std::find_if(
                        combatInstances.begin(),
                        combatInstances.end(),
                        [&player](const auto &combatState) {
                            return (combatState.attackerID == player.getId());
                        }
                );

                combat_it->endRound();

                if (player.getHealth() == 0) {
                    message << this->loseEvent(player, npc);
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

            this->exitCombat(player);
            message << "You successfully flee to the " << direction << ".\n";

        } else {
            auto opponentId = this->getOpponentId(player);
            auto &npc = this->worldHandler.findRoom(roomId).getNpcById(opponentId);

            message << "\n"
                    << "You attempt to flee, but fail. ("
                    << (BASE_FLEE_CHANCE * doors.size() * 100) << "% chance of success)\n";

            message << this->inflictDamage(player);

            auto combat_it = std::find_if(
                    combatInstances.begin(),
                    combatInstances.end(),
                    [&player](const auto &combatState) {
                        return (combatState.attackerID == player.getId());
                    }
            );

            combat_it->endRound();

            if (player.getHealth() == 0) {
                message << this->loseEvent(player, npc);
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
    CombatHandler::replaceWithDecoy(const Player &player) {
        if (!isInCombat(player)) {
            return;
        }

        auto characterId = player.getId();
        model::ID decoyId = -player.getId();
        std::vector<std::string> keywords = {boost::algorithm::to_lower_copy(player.getUsername())};
        std::vector<std::string> description = {"Upon closer inspection, you realize this is just a decoy version of " +
                                                player.getUsername() + "."};
        std::string shortDescription = "'" + player.getUsername() + "'";

        NPC decoy{decoyId, keywords, description, shortDescription, {}};

        auto client = this->accountHandler.getClientByPlayerId(characterId);
        auto roomId = this->accountHandler.getRoomIdByClient(client);
        this->worldHandler.findRoom(roomId).addNPC(decoy);

        for (auto &combatInstance : this->combatInstances) {
            if (combatInstance.attackerID == characterId) {
                combatInstance.attackerID = decoyId;
                break;
            };
        }
    };


    void
    CombatHandler::handleLogout(const Connection &client) {
        auto &player = this->accountHandler.getPlayerByClient(client);

        if (this->isInCombat(player)) {
            auto roomId = this->accountHandler.getRoomIdByClient(client);
            auto npcId = this->getOpponentId(player);
            auto &npc = this->worldHandler.findRoom(roomId).getNpcById(npcId);

            this->exitCombat(player);
            npc.setHealth(Character::STARTING_HEALTH);
        }
    }


    void
    CombatHandler::processCycle(std::deque<Message> &messages) {
        std::vector<Character> defeatedCharacters;

        for (auto &combatInstance : this->combatInstances) {
            if (combatInstance.roundCyclesRemaining > 0) {
                combatInstance.decrement();

            } else {
                auto client = this->accountHandler.getClientByPlayerId(combatInstance.attackerID);

                if (client.id != AccountHandler::INVALID_ID) {
                    auto &player = this->accountHandler.getPlayerByClient(client);
                    auto roomId = this->accountHandler.getRoomIdByClient(client);
                    auto &npc = this->worldHandler.findRoom(roomId).getNpcById(combatInstance.defenderID);

                    std::ostringstream message;
                    message << "\n" << this->inflictDamage(player);

                    if (player.getHealth() == 0) {
                        message << this->loseEvent(player, npc);
                    }

                    messages.push_back({client, message.str()});
                    combatInstance.endRound();

                } else {
                    model::ID roomId = 0;
                    // attacker was replaced by dummy
                    auto areas = this->worldHandler.getWorld().getAreas();

                    for (auto &area : areas) {

                        auto rooms = area.getRooms();
                        for (auto &room : rooms) {

                            auto npcs = room.getNpcs();
                            for (auto &npc : npcs) {
                                if (npc.getId() == combatInstance.attackerID) {
                                    roomId = room.getId();
                                }
                            }
                        }
                    }

                    if (roomId == 0) {
                        return;
                    }

                    auto &dummy = this->worldHandler.findRoom(roomId).getNpcById(combatInstance.attackerID);
                    auto &npc = this->worldHandler.findRoom(roomId).getNpcById(combatInstance.defenderID);

                    std::ostringstream message;
                    this->inflictDamage(dummy);

                    if (dummy.getHealth() == 0) {
                        // Delete dummy
                        defeatedCharacters.push_back(dummy);
                        auto &npcs = this->worldHandler.findRoom(roomId).getNpcs();
                        auto npc_it = std::find_if(
                                npcs.begin(),
                                npcs.end(),
                                [&dummy](const auto &roomNpc) {
                                    return roomNpc.getId() == dummy.getId();
                                });
                        npcs.erase(npc_it);
                        npc.setHealth(Character::STARTING_HEALTH);
                    }

                    combatInstance.endRound();

                }
            }
        }

        for (const auto &defeatedCharacter : defeatedCharacters) {
            this->exitCombat(defeatedCharacter);
        }
    }
}