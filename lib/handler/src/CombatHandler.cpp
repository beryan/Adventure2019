//
// Created by jnhkm on 2019-03-10.
//

#include "CombatHandler.h"
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
    CombatHandler::enterCombat(const Character &attacker, const NPC &defender) {
        this->combatInstances.emplace_back(CombatInstance{attacker.getId(), defender.getUniqueId()});
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
        auto npc = this->worldHandler.findRoom(roomId).getNpcByUniqueId(npcId);
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
        damage = std::max(damage, 0);

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
                return (roomNpc.getUniqueId() == npc.getUniqueId());
            });
        npcs.erase(npc_it);

        return message.str();
    }


    std::string
    CombatHandler::loseEvent(Player &player, NPC& npc) {
        std::ostringstream message;
        this->exitCombat(player, npc);

        npc.setHealth(Character::STARTING_HEALTH);
        player.setHealth(Character::STARTING_HEALTH);

        auto startRoomId = Player::STARTING_LOCATION;
        // Check if starting room actually exists, pick first room in first area otherwise
        if (!this->worldHandler.roomExists(Player::STARTING_LOCATION)) {
            assert(!this->worldHandler.getWorld().getAreas().empty());
            auto firstArea = this->worldHandler.getWorld().getAreas().at(0);
            assert(!firstArea.getRooms().empty());
            startRoomId = firstArea.getRooms().at(0).getId();
        }

        auto currentRoomId = player.getCurrRoomID();
        auto startRoom = this->worldHandler.findRoom(startRoomId);
        this->worldHandler.movePlayer(player.getId(), currentRoomId, startRoomId);
        player.setCurrRoomID(startRoomId);

        message << "You have fallen in battle and respawned.\n";

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

            bool playerInCombat = this->isInCombat(player.getId());
            bool npcInCombat = this->isInCombat(npc.getUniqueId());
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
                return message.str();
            }

            message << this->inflictDamage(player);

            if (player.getHealth() == 0) {
                message << this->loseEvent(player, npc);
                return message.str();
            }

            auto combat_it = std::find_if(
                this->combatInstances.begin(),
                this->combatInstances.end(),
                [&player](const auto &combatState) {
                    return (combatState.attackerID == player.getId());
                }
            );

            if (combat_it != this->combatInstances.end()) {
                combat_it->endRound();
            }

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

        if (!this->isInCombat(player.getId())) {
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
                auto &npc = this->worldHandler.findRoom(roomId).getNpcByUniqueId(opponentId);

                message << "\n"
                        << "You attempt to flee, but fail. ("
                        << (BASE_FLEE_CHANCE / 2 * 100) << "% chance of success)\n";

                message << this->inflictDamage(player);

                if (player.getHealth() == 0) {
                    message << this->loseEvent(player, npc);
                    return message.str();
                }

                auto combat_it = std::find_if(
                    this->combatInstances.begin(),
                    this->combatInstances.end(),
                    [&player](const auto &combatState) {
                        return (combatState.attackerID == player.getId());
                    }
                );

                if (combat_it != this->combatInstances.end()) {
                    combat_it->endRound();
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
            auto &npc = this->worldHandler.findRoom(roomId).getNpcByUniqueId(opponentId);

            message << "\n"
                    << "You attempt to flee, but fail. ("
                    << (BASE_FLEE_CHANCE * doors.size() * 100) << "% chance of success)\n";

            message << this->inflictDamage(player);

            if (player.getHealth() == 0) {
                message << this->loseEvent(player, npc);
                return message.str();
            }

            auto combat_it = std::find_if(
                combatInstances.begin(),
                combatInstances.end(),
                [&player](const auto &combatState) {
                    return (combatState.attackerID == player.getId());
                }
            );

            if (combat_it != this->combatInstances.end()) {
                combat_it->endRound();
            }
        }

        return message.str();
    }


    bool
    CombatHandler::isInCombat(model::ID id) {
        auto combat_it = std::find_if(
            this->combatInstances.begin(),
            this->combatInstances.end(),
            [&id](const auto &combatState) {
                return (combatState.attackerID == id || combatState.defenderID == id);
            }
        );

        return combat_it != this->combatInstances.end();
    };


    bool
    CombatHandler::areInCombat(const Character &attacker, const NPC &defender) {
        auto attackerId = attacker.getId();
        auto defenderId = defender.getUniqueId();

        auto combat_it = std::find(
                this->combatInstances.begin(),
                this->combatInstances.end(),
                CombatInstance{attackerId, defenderId}
        );

        return combat_it != this->combatInstances.end();
    }


    void
    CombatHandler::exitCombat(const Character &character, const NPC &npc) {
        auto it = std::find(
            this->combatInstances.begin(),
            this->combatInstances.end(),
            CombatInstance{character.getId(), npc.getUniqueId()});

        if (it != combatInstances.end()) {
            combatInstances.erase(it);
        }
    }


    void
    CombatHandler::exitCombat(const Player &player) {
        auto playerId = player.getId();
        auto combat_it = std::find_if(
            this->combatInstances.begin(),
            this->combatInstances.end(),
            [&playerId](const auto &combatState) {
                return (combatState.attackerID == playerId);
            }
        );

        if (combat_it != combatInstances.end()) {
            combatInstances.erase(combat_it);
        }
    }


    void
    CombatHandler::exitCombat(const NPC &npc) {
        auto npcUniqueId= npc.getUniqueId();
        auto combat_it = std::find_if(
                this->combatInstances.begin(),
                this->combatInstances.end(),
                [&npcUniqueId](const auto &combatState) {
                    return (combatState.attackerID == npcUniqueId || combatState.defenderID == npcUniqueId);
                }
        );

        if (combat_it != combatInstances.end()) {
            combatInstances.erase(combat_it);
        }
    }


    model::ID
    CombatHandler::getOpponentId(const Player &player) {
        auto playerId = player.getId();

        auto combat_it = std::find_if(
            this->combatInstances.begin(),
            this->combatInstances.end(),
            [&playerId](const auto &combatInstance) {
                return (combatInstance.attackerID == playerId);
            }
        );

        if (combat_it == this->combatInstances.end()) {
            return 0;

        } else  {
            return combat_it->defenderID;
        }
    }


    model::ID
    CombatHandler::getOpponentId(const NPC &npc) {
        auto npcUniqueId = npc.getUniqueId();

        auto combat_it = std::find_if(
                this->combatInstances.begin(),
                this->combatInstances.end(),
                [&npcUniqueId](const auto &combatInstance) {
                    return (combatInstance.attackerID == npcUniqueId || combatInstance.defenderID == npcUniqueId);
                }
        );

        if (combat_it == this->combatInstances.end()) {
            return 0;
        }

        if (combat_it->attackerID == npcUniqueId) {
            return combat_it->defenderID;

        } else {
            return combat_it->attackerID;
        }
    }


    void
    CombatHandler::replaceWithDecoy(const Player &player) {
        if (!isInCombat(player.getId())) {
            return;
        }

        auto characterId = player.getId();
        model::ID decoyId = -player.getId();
        std::vector<std::string> keywords = {boost::algorithm::to_lower_copy(player.getUsername())};
        std::vector<std::string> description = {"Upon closer inspection, you realize this is just a decoy version of " +
                                                player.getUsername() + "."};
        std::string shortDescription = "'" + player.getUsername() + "'";

        NPC decoy{decoyId, keywords, description, shortDescription, {}};
        decoy.setUniqueId(decoyId);

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
    CombatHandler::removeActiveDecoy(const Player &player) {
        model::ID decoyId = -player.getId();

        auto combat_it = std::find_if(
            this->combatInstances.begin(),
            this->combatInstances.end(),
            [&decoyId](const auto &combatState) {
                return (combatState.attackerID == decoyId);
            });

        if (combat_it != this->combatInstances.end()) {
            this->combatInstances.erase(combat_it);

            model::ID decoyRoomId;
            // find and remove decoy in world
            auto areas = this->worldHandler.getWorld().getAreas();
            for (auto &area : areas) {
                auto &rooms = area.getRooms();
                for (auto &room : rooms) {
                    auto &npcs = room.getNpcs();
                    for (auto &npc : npcs) {
                        if (npc.getId() == decoyId) {
                            decoyRoomId = room.getId();
                        }
                    }
                }
            }

            auto &decoyRoom = this->worldHandler.findRoom(decoyRoomId);
            auto &npcs = decoyRoom.getNpcs();

            auto npc_it = std::find_if(
                    npcs.begin(),
                    npcs.end(),
                    [&decoyId](const auto &npc) {
                        return (npc.getId() == decoyId);
                    });

            if (npc_it != npcs.end()){
                npcs.erase(npc_it);
            }
        }
    };


    void
    CombatHandler::handleLogout(const Connection &client) {
        auto &player = this->accountHandler.getPlayerByClient(client);

        if (this->isInCombat(player.getId())) {
            auto roomId = this->accountHandler.getRoomIdByClient(client);
            auto npcId = this->getOpponentId(player);
            auto &npc = this->worldHandler.findRoom(roomId).getNpcByUniqueId(npcId);

            this->exitCombat(player);
            npc.setHealth(Character::STARTING_HEALTH);
        }
    }


    void
    CombatHandler::processCycle(std::deque<Message> &messages) {
        for (auto &combatInstance : this->combatInstances) {
            if (combatInstance.roundCyclesRemaining > 0) {
                combatInstance.decrement();

            } else {
                auto client = this->accountHandler.getClientByPlayerId(combatInstance.attackerID);

                if (client.id != AccountHandler::INVALID_ID) {
                    auto &player = this->accountHandler.getPlayerByClient(client);
                    auto roomId = this->accountHandler.getRoomIdByClient(client);
                    auto &npc = this->worldHandler.findRoom(roomId).getNpcByUniqueId(combatInstance.defenderID);

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

                    // decoy does not exist anywhere, process next combat instance
                    if (roomId == 0) {
                        continue;
                    }

                    auto &decoy= this->worldHandler.findRoom(roomId).getNpcByUniqueId(combatInstance.attackerID);
                    auto &npc = this->worldHandler.findRoom(roomId).getNpcByUniqueId(combatInstance.defenderID);

                    std::ostringstream message;
                    this->inflictDamage(decoy);

                    if (decoy.getHealth() == 0) {
                        // Delete decoy
                        this->exitCombat(decoy);

                        auto &npcs = this->worldHandler.findRoom(roomId).getNpcs();
                        auto npc_it = std::find_if(
                            npcs.begin(),
                            npcs.end(),
                            [&decoy](const auto &roomNpc) {
                                return roomNpc.getId() == decoy.getId();
                            });
                        npcs.erase(npc_it);
                        npc.setHealth(Character::STARTING_HEALTH);
                    }

                    combatInstance.endRound();

                }
            }
        }
    }
}