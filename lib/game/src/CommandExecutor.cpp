//
// Created by Stephen Wanhella on 3/14/2019.
//

#include <CommandExecutor.h>
#include <iostream>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/join.hpp>
#include "Character.h"

using game::CommandExecutor;
using model::NPC;
using model::Room;

constexpr auto ALIAS_LIST = "list";
constexpr auto ALIAS_SET = "set";
constexpr auto ALIAS_SET_GLOBAL = "set-global";
constexpr auto ALIAS_CLEAR = "clear";
constexpr auto ALIAS_CLEAR_GLOBAL = "clear-global";
constexpr auto ALIAS_HELP = "help";
constexpr auto ALIAS_SET_NUM_PARAMS = 3;
constexpr auto ALIAS_CLEAR_NUM_PARAMS = 2;
constexpr auto ALIAS_LIST_NUM_PARAMS = 1;

CommandExecutor::CommandExecutor(ConnectionHandler &connectionHandler, AccountHandler &accountHandler,
                                 AvatarHandler &avatarHandler, MagicHandler &magicHandler, CombatHandler &combatHandler,
                                 WorldHandler &worldHandler, AliasManager &aliasManager, CommandParser &commandParser)
        : connectionHandler(connectionHandler),
          accountHandler(accountHandler),
          magicHandler(magicHandler),
          combatHandler(combatHandler),
          worldHandler(worldHandler),
          aliasManager(aliasManager),
          commandParser(commandParser) {}

std::vector<Message> CommandExecutor::executeCommand(const Connection &client, const Command &command,
                                                     const std::vector<std::string> &params) {
    std::vector<Message> messages = {};
    std::ostringstream tempMessage;

    switch (command) {
        case Command::Help: {
            tempMessage << help();
            break;
        }

        case Command::Say: {
            auto message = boost::algorithm::join(params, " ");
            return say(client, message);
        }

        case Command::Yell: {
            auto message = boost::algorithm::join(params, " ");
            return yell(client, message);
        }

        case Command::Tell: {
            auto messageRange = std::vector<std::string>(params.begin() + 1, params.end());
            auto message = boost::algorithm::join(messageRange, " ");
            return tell(client, params[0], message);
        }

        case Command::Chat: {
            auto message = boost::algorithm::join(params, " ");
            return chat(client, message);
        }

        case Command::Look: {
            if (params.empty() || params[0].empty()) {
                tempMessage << look(client);
            } else {
                tempMessage << examine(client, params[0]);
            }
            break;
        }

        case Command::Examine: {
            tempMessage << examine(client, params[0]);
            break;
        }

        case Command::Exits: {
            tempMessage << exits(client);
            break;
        }

        case Command::Move: {
            tempMessage << move(client, boost::algorithm::to_lower_copy(params[0]));
            break;
        }

        case Command::Talk: {
            tempMessage << talk(client, params[0]);
            break;
        }

        case Command::Take: {
            tempMessage << take(client, params[0]);
            break;
        }

        case Command::Drop: {
            tempMessage << drop(client, params[0]);
            break;
        }

        case Command::Wear: {
            tempMessage << wear(client, params[0]);
            break;
        }

        case Command::Remove: {
            tempMessage << remove(client, params[0]);
            break;
        }

        case Command::Give: {
            return give(client, params[0], params[1]);
        }

        case Command::Attack: {
            tempMessage << combatHandler.attack(client, params[0]);
            break;
        }

        case Command::Flee: {
            tempMessage << combatHandler.flee(client);
            break;
        }

        case Command::Cast: {
            auto spellName = !params.empty() ? params[0] : "";
            auto targetName = params.size() >= 2 ? params[1] : "";
            return magicHandler.castSpell(client, spellName, targetName);
        }

        case Command::Spells: {
            tempMessage << magicHandler.getSpells();
            break;
        }

        case Command::Status: {
            tempMessage << status(client);
            break;
        }

        case Command::Inventory: {
            tempMessage << accountHandler.getPlayerByClient(client).getMutableInventory();
            break;
        }

        case Command::Equipment: {
            tempMessage << accountHandler.getPlayerByClient(client).getMutableEquipment();
            break;
        }

        case Command::Alias: {
            tempMessage << alias(client, params);
            break;
        }

        case Command::Build: {
            if (this->magicHandler.isBodySwapped(client)) {
                tempMessage << "You cannot use this command while body swapped.\n";
            } else {
                tempMessage << build();
            }
            break;
        }

        case Command::Builder: {
            if (this->magicHandler.isBodySwapped(client)) {
                tempMessage << "You cannot use this command while body swapped.\n";
            } else {
                tempMessage << builder(params[0]);
            }
            break;
        }

        case Command::Acreate: {
            if (this->magicHandler.isBodySwapped(client)) {
                tempMessage << "You cannot use this command while body swapped.\n";
            } else {
                auto name = boost::algorithm::join(params, " ");
                if (this->worldHandler.createArea(name)) {
                    tempMessage << "Area successfully created.\n";
                } else {
                    tempMessage << "Failed to create area.\n";
                }
            }
            break;
        }

        case Command::Rcreate: {
            if (this->magicHandler.isBodySwapped(client)) {
                tempMessage << "You cannot use this command while body swapped.\n";
            } else {
                if (this->worldHandler.createRoom(params)) {
                    tempMessage << "Room successfully created.\n";
                } else {
                    tempMessage << "Failed to create room.\n";
                }
            }
            break;
        }

        case Command::Ocreate: {
            if (this->magicHandler.isBodySwapped(client)) {
                tempMessage << "You cannot use this command while body swapped.\n";
            } else {
                auto roomId = this->accountHandler.getRoomIdByClient(client);
                if (this->worldHandler.createObject(roomId, params)) {
                    tempMessage << "Object successfully created.\n";
                } else {
                    tempMessage << "Failed to create object.\n";
                }
            }
            break;
        }

        case Command::Ncreate: {
            if (this->magicHandler.isBodySwapped(client)) {
                tempMessage << "You cannot use this command while body swapped.\n";
            } else {
                auto roomId = this->accountHandler.getRoomIdByClient(client);
                if (this->worldHandler.createNpc(roomId, params)) {
                    tempMessage << "NPC successfully created.\n";
                } else {
                    tempMessage << "Failed to create NPC.\n";
                }
            }
            break;
        }

        case Command::Aedit: {
            if (this->magicHandler.isBodySwapped(client)) {
                tempMessage << "You cannot use this command while body swapped.\n";
            } else {
                auto roomId = this->accountHandler.getRoomIdByClient(client);
                if (this->worldHandler.editArea(roomId, params)) {
                    tempMessage << "Area successfully edited.\n";
                } else {
                    tempMessage << "Failed to edit Area.\n";
                }
            }
            break;
        }

        case Command::Redit: {
            if (this->magicHandler.isBodySwapped(client)) {
                tempMessage << "You cannot use this command while body swapped.\n";
            } else {
                auto roomId = this->accountHandler.getRoomIdByClient(client);
                if (this->worldHandler.editRoom(roomId, params)) {
                    tempMessage << "Room successfully edited.\n";
                } else {
                    tempMessage << "Failed to edit Room.\n";
                }
            }
            break;
        }

        case Command::Oedit: {
            if (this->magicHandler.isBodySwapped(client)) {
                tempMessage << "You cannot use this command while body swapped.\n";
            } else {
                auto roomId = this->accountHandler.getRoomIdByClient(client);
                if (this->worldHandler.editObject(roomId, params)) {
                    tempMessage << "Object successfully edited.\n";
                } else {
                    tempMessage << "Failed to edit Object.\n";
                }
            }
            break;
        }

        case Command::Nedit: {
            if (this->magicHandler.isBodySwapped(client)) {
                tempMessage << "You cannot use this command while body swapped.\n";
            } else {
                auto roomId = this->accountHandler.getRoomIdByClient(client);
                if (this->worldHandler.editNpc(roomId, params)) {
                    tempMessage << "NPC successfully edited.\n";
                } else {
                    tempMessage << "Failed to edit NPC.\n";
                }
            }
            break;
        }

        case Command::Oreset: {
            if (this->magicHandler.isBodySwapped(client)) {
                tempMessage << "You cannot use this command while body swapped.\n";
            } else {
                auto roomId = this->accountHandler.getRoomIdByClient(client);
                if (this->worldHandler.createObjectReset(roomId, params)) {
                    tempMessage << "Reset successfully created.\n";
                } else {
                    tempMessage << "Failed to create reset.\n";
                }
            }
            break;
        }

        case Command::Nreset: {
            if (this->magicHandler.isBodySwapped(client)) {
                tempMessage << "You cannot use this command while body swapped.\n";
            } else {
                auto roomId = this->accountHandler.getRoomIdByClient(client);
                if (this->worldHandler.createNpcReset(roomId, params)) {
                    tempMessage << "Reset successfully created.\n";
                } else {
                    tempMessage << "Failed to create reset.\n";
                }
            }
            break;
        }

        case Command::Alist: {
            if (this->magicHandler.isBodySwapped(client)) {
                tempMessage << "You cannot use this command while body swapped.\n";
            } else {
                tempMessage << alist();
            }
            break;
        }

        case Command::Rlist: {
            if (this->magicHandler.isBodySwapped(client)) {
                tempMessage << "You cannot use this command while body swapped.\n";
            } else {
                tempMessage << rlist(params[0]);
            }
            break;
        }

        case Command::Olist: {
            if (this->magicHandler.isBodySwapped(client)) {
                tempMessage << "You cannot use this command while body swapped.\n";
            } else {
                tempMessage << olist(params[0]);
            }
            break;
        }

        case Command::Nlist: {
            if (this->magicHandler.isBodySwapped(client)) {
                tempMessage << "You cannot use this command while body swapped.\n";
            } else {
                tempMessage << nlist(params[0]);
            }
            break;
        }

        case Command::Ashow: {
            if (this->magicHandler.isBodySwapped(client)) {
                tempMessage << "You cannot use this command while body swapped.\n";
            } else {
                if (params[0].empty()) {
                    auto roomId = this->accountHandler.getRoomIdByClient(client);
                    tempMessage << this->worldHandler.findArea(roomId);
                } else {
                    tempMessage << "Invalid format.\n";
                }
            }
            break;
        }

        case Command::Rshow: {
            if (this->magicHandler.isBodySwapped(client)) {
                tempMessage << "You cannot use this command while body swapped.\n";
            } else {
                if (params[0].empty()) {
                    auto roomId = this->accountHandler.getRoomIdByClient(client);
                    tempMessage << this->worldHandler.findRoom(roomId).toString();
                } else {
                    tempMessage << "Invalid format.\n";
                }
            }
            break;
        }

        case Command::Oshow: {
            if (this->magicHandler.isBodySwapped(client)) {
                tempMessage << "You cannot use this command while body swapped.\n";
            } else {
                tempMessage << oshow(client, params[0]);
            }
            break;
        }

        case Command::Nshow: {
            if (this->magicHandler.isBodySwapped(client)) {
                tempMessage << "You cannot use this command while body swapped.\n";
            } else {
                tempMessage << nshow(client, params[0]);
            }
            break;
        }

        case Command::Goto: {
            if (this->magicHandler.isBodySwapped(client)) {
                tempMessage << "You cannot use this command while body swapped.\n";
            } else {
                tempMessage << go(client, params[0]);
            }
            break;
        }

        case Command::Clear: {
            if (this->magicHandler.isBodySwapped(client)) {
                tempMessage << "You cannot use this command while body swapped.\n";
            } else {
                auto roomId = this->accountHandler.getRoomIdByClient(client);
                auto &room = this->worldHandler.findRoom(roomId);
                auto players = room.getPlayersInRoom();

                // Disengage all players in room from combat
                for (const auto &playerId : players) {
                    if (this->combatHandler.isInCombat(playerId)) {
                        this->combatHandler.exitCombat(playerId);
                    }
                }

                this->worldHandler.clear(roomId);
                tempMessage << "Room cleared.\n";
            }
            break;
        }

        case Command::Reset: {
            if (this->magicHandler.isBodySwapped(client)) {
                tempMessage << "You cannot use this command while body swapped.\n";
            } else {
                auto roomId = this->accountHandler.getRoomIdByClient(client);
                this->worldHandler.resetArea(roomId);
                tempMessage << "Area reset.\n";
            }
            break;
        }

        default: {
            tempMessage << "\nEnter " << "\"" << this->commandParser.getStringForCommand(Command::Help)
                        << "\" for a full list of commands\n";
            break;
        }
    }

    messages.push_back({client, tempMessage.str()});

    return messages;
}

std::string CommandExecutor::help() {
    std::ostringstream tempMessage;
    tempMessage << "\n"
                << "********\n"
                << "* HELP *\n"
                << "********\n"
                << "\n"
                << "COMMANDS:\n"
                << "  - " << commandParser.getStringForCommand(Command::Help)
                << " (shows this help interface)\n"
                << "  - " << commandParser.getStringForCommand(Command::Say)
                << " [message] (sends [message] to players in the same room)\n"
                << "  - " << commandParser.getStringForCommand(Command::Yell)
                << " [message] (sends [message] loud enough to be heard by players in adjacent rooms)\n"
                << "  - " << commandParser.getStringForCommand(Command::Tell)
                << " [username] [message] (sends [message] to [username] in the game)\n"
                << "  - " << commandParser.getStringForCommand(Command::Chat)
                << " [message] (sends [message] to global chat)\n"
                << "  - " << commandParser.getStringForCommand(Command::Look)
                << " (displays current location information)\n"
                << "  - " << commandParser.getStringForCommand(Command::Exits)
                << " (displays exits from current location)\n"
                << "  - " << commandParser.getStringForCommand(Command::Move)
                << " [direction] (moves you in the direction specified)\n"
                << "  - " << commandParser.getStringForCommand(Command::Examine)
                << " [keyword] (examines something or someone)\n"
                << "  - " << commandParser.getStringForCommand(Command::Take)
                << " [keyword] (places item in your inventory)\n"
                << "  - " << commandParser.getStringForCommand(Command::Drop)
                << " [keyword] (drops item from inventory/equipment)\n"
                << "  - " << commandParser.getStringForCommand(Command::Wear)
                << " [keyword] (equips item from your inventory)\n"
                << "  - " << commandParser.getStringForCommand(Command::Remove)
                << " [keyword] (unequips item to your inventory)\n"
                << "  - " << commandParser.getStringForCommand(Command::Give)
                << " [username] [keyword] (gives item to username)\n"
                << "  - " << commandParser.getStringForCommand(Command::Talk)
                << " [keyword] (interacts with NPC)\n"
                << "  - " << commandParser.getStringForCommand(Command::Attack)
                << " [keyword] (attack an NPC. Suffix a number to the keyword to target specific NPCs with the same name)\n"
                << "  - " << commandParser.getStringForCommand(Command::Flee)
                << " (attempt to escape from combat, moving in a random direction)\n"
                << "  - " << commandParser.getStringForCommand(Command::Spells)
                << " (displays available magic spells)\n"
                << "  - " << commandParser.getStringForCommand(Command::Cast)
                << " [spell] [target] (casts a spell on a target)\n"
                << "  - " << commandParser.getStringForCommand(Command::Inventory)
                << " (displays your inventory)\n"
                << "  - " << commandParser.getStringForCommand(Command::Equipment)
                << " (displays your equipment)\n"
                << "  - " << commandParser.getStringForCommand(Command::Status)
                << " (displays your health, combat attributes, and current status ailments)\n"
                << "  - " << this->commandParser.getStringForCommand(Command::Build)
                << " (displays available world building commands)\n"
                << "  - " << this->commandParser.getStringForCommand(Command::Builder)
                << " [username] (toggles build command access for username)\n"
                << "  - " << commandParser.getStringForCommand(Command::Alias)
                << " (aliases a command. Type \""
                << commandParser.getStringForCommand(Command::Alias) << " help\" for details)\n"
                << "  - " << commandParser.getStringForCommand(Command::Logout)
                << " (logs you out of the game)\n"
                << "  - " << commandParser.getStringForCommand(Command::Quit)
                << " (disconnects you from the game server)\n"
                << "  - " << commandParser.getStringForCommand(Command::Shutdown)
                << " (shuts down the game server)\n";
    return tempMessage.str();
}

std::vector<Message> CommandExecutor::say(const Connection &client, std::string &message) {
    std::vector<Message> messages = {};
    auto roomId = accountHandler.getRoomIdByClient(client);
    auto playerIds = worldHandler.findRoom(roomId).getPlayersInRoom();

    if (magicHandler.isConfused(client)) {
        magicHandler.confuseSpeech(message);
    }

    for (const auto &playerId : playerIds) {
        auto connection = accountHandler.getClientByPlayerId(playerId);

        std::ostringstream sayMessage;
        sayMessage << accountHandler.getUsernameByClient(client) << " says> " << message << "\n";

        messages.push_back({connection, sayMessage.str()});
    }

    return messages;
}

std::vector<Message> CommandExecutor::yell(const Connection &client, std::string &message) {
    std::vector<Message> messages;
    auto roomId = accountHandler.getRoomIdByClient(client);
    auto playerIds = worldHandler.getNearbyPlayerIds(roomId);

    if (magicHandler.isConfused(client)) {
        magicHandler.confuseSpeech(message);
    }

    for (const auto &playerId : playerIds) {
        auto connection = accountHandler.getClientByPlayerId(playerId);

        std::ostringstream sayMessage;
        sayMessage << accountHandler.getUsernameByClient(client) << " yells> " << message << "\n";

        messages.push_back({connection, sayMessage.str()});
    }

    return messages;
}

std::vector<Message>
CommandExecutor::tell(const Connection &client, const std::string &username, std::string &message) {
    std::vector<Message> messages;

    if (this->magicHandler.isConfused(client)) {
        this->magicHandler.confuseSpeech(message);
    }

    for (auto connection: connectionHandler.getClients()) {
        auto receiver = this->accountHandler.getUsernameByClient(connection);

        if (receiver == username) {
            auto sender = this->accountHandler.getUsernameByClient(client);

            std::ostringstream toMessage;
            std::ostringstream fromMessage;

            toMessage << "To " << receiver << "> " << message << "\n";
            fromMessage << "From " + sender + "> " << message << "\n";;

            messages.push_back({client, toMessage.str()});
            messages.push_back({connection, fromMessage.str()});

            return messages;
        }
    }

    std::ostringstream tempMessage;
    tempMessage << "Unable to find online user \"" << username << "\".\n";
    messages.push_back({client, tempMessage.str()});
    return messages;
}

std::vector<Message> CommandExecutor::chat(const Connection &client, std::string &message) {
    std::vector<Message> messages = {};
    if (magicHandler.isConfused(client)) {
        magicHandler.confuseSpeech(message);
    }

    for (auto connection : connectionHandler.getClients()) {
        if (accountHandler.isLoggedIn(connection)) {
            std::ostringstream chatMessage;
            chatMessage << accountHandler.getUsernameByClient(client) << "> " << message << "\n";
            messages.push_back({connection, chatMessage.str()});
        }
    }

    return messages;
}

std::string CommandExecutor::look(const Connection &client) {
    std::ostringstream tempMessage;
    auto roomId = accountHandler.getRoomIdByClient(client);
    auto room = worldHandler.findRoom(roomId);
    tempMessage << room << "Players:\n";
    for (const auto &id : room.getPlayersInRoom()) {
        tempMessage << "- " << accountHandler.getUsernameByPlayerId(id) << std::endl;
    }
    return tempMessage.str();
}

std::string CommandExecutor::examine(const Connection &client, const std::string &keyword) {
    std::ostringstream tempMessage;
    auto room = worldHandler.findRoom(accountHandler.getRoomIdByClient(client));
    auto objects = room.getObjects();
    auto npcs = room.getImmutableNpcs();
    auto extras = room.getExtras();

    auto assumedClient = this->accountHandler.getClientByUsername(keyword);
    if (assumedClient.id != AccountHandler::INVALID_ID) {
        auto playerRoom = this->accountHandler.getRoomIdByClient(client);
        auto examinedPlayerRoom = this->accountHandler.getRoomIdByClient(assumedClient);

        if (playerRoom == examinedPlayerRoom) {
            auto examinedPlayer = this->accountHandler.getPlayerByClient(assumedClient);
            tempMessage << examinedPlayer.getDescription();

            return tempMessage.str();
        }
    }

    if (containsKeyword(objects, keyword)) {
        auto obj = getItemByKeyword(objects, keyword);

        for (const auto &str : obj.getLongDescription()) {
            tempMessage << str << std::endl;
        }

    } else if (containsKeyword(npcs, keyword)) {
        auto npc = getItemByKeyword(npcs, keyword);

        for (const auto &str : npc.getDescription()) {
            tempMessage << str << std::endl;
        }

    } else if (containsKeyword(extras, keyword)) {
        auto extra = getItemByKeyword(extras, keyword);

        for (const auto &str : extra.getExtraDescriptions()) {
            tempMessage << str << std::endl;
        }

    } else {
        tempMessage << "Invalid keyword.\n";
    }
    return tempMessage.str();
}

std::string CommandExecutor::exits(const Connection &client) {
    std::ostringstream tempMessage;
    auto roomID = this->accountHandler.getRoomIdByClient(client);
    tempMessage << this->worldHandler.findRoom(roomID).doorsToString();
    return tempMessage.str();
}

std::string CommandExecutor::move(const Connection &client, const std::string &dir) {
    std::ostringstream tempMessage;
    auto &player = this->accountHandler.getPlayerByClient(client);
    auto roomId = accountHandler.getRoomIdByClient(client);

    if (this->combatHandler.isInCombat(player.getId())) {
        tempMessage << "You can't expect to just stroll out of here with someone attacking you!"
                    << " Perhaps you should flee instead.\n";

    } else if (worldHandler.isValidDirection(roomId, dir)) {
        auto destinationId = worldHandler.getDestination(roomId, dir);

        if (this->worldHandler.roomExists(destinationId)) {
            worldHandler.movePlayer(player.getId(), roomId, destinationId);
            accountHandler.setRoomIdByClient(client, destinationId);
            tempMessage << worldHandler.findRoom(destinationId).descToString();

        } else {
            tempMessage << "You can't move that way!\n";
        }

    } else {
        tempMessage << "Invalid direction!\n";
    }

    return tempMessage.str();
}

std::string CommandExecutor::talk(const Connection &client, const std::string &keyword) {
    std::ostringstream tempMessage;
    auto room = worldHandler.findRoom(accountHandler.getRoomIdByClient(client));
    auto npcs = room.getImmutableNpcs();

    if (!containsKeyword(npcs, keyword)) {
        tempMessage << "Invalid keyword.\n";
    }

    auto &player = this->accountHandler.getPlayerByClient(client);
    auto npc = getItemByKeyword(npcs, keyword);

    if (this->combatHandler.isInCombat(player.getId())) {
        tempMessage << "You are too busy fighting to talk!\n";
        return tempMessage.str();
    }

    bool npcInCombat = this->combatHandler.isInCombat(npc.getUniqueId());
    bool inCombatWithPlayer = this->combatHandler.areInCombat(player.getId(), npc.getUniqueId());
    if (npcInCombat && !inCombatWithPlayer) {
        auto otherPlayerId = this->combatHandler.getOpponentId(npc.getUniqueId());
        auto otherPlayerName = this->accountHandler.getUsernameByPlayerId(otherPlayerId);
        if (otherPlayerName.empty()) {
            // Decoy npc;
            otherPlayerName = room.getNpcByUniqueId(otherPlayerId).getShortDescription();
        }

        tempMessage << npc.getShortDescription() << " is busy fighting " << otherPlayerName << ".\n";
        return tempMessage.str();
    }


    for (const auto &str : npc.getLongDescription()) {
        tempMessage << str << std::endl;
    }

    return tempMessage.str();
}

std::string CommandExecutor::take(const Connection &client, const std::string &keyword) {
    std::ostringstream tempMessage;
    auto roomId = accountHandler.getRoomIdByClient(client);
    Room &room = worldHandler.findRoom(roomId);
    auto objects = room.getObjects();

    if (containsKeyword(objects, keyword)) {
        auto item = getItemByKeyword(objects, keyword);
        auto &player = accountHandler.getPlayerByClient(client);
        worldHandler.removeItem(roomId, item.getId());
        playerHandler.pickupItem(player, item);
        tempMessage << "Item taken successfully.\n";
    } else {
        tempMessage << "Invalid keyword.\n";
    }
    return tempMessage.str();
}

std::string CommandExecutor::drop(const Connection &client, const std::string &keyword) {
    std::ostringstream tempMessage;
    auto &player = accountHandler.getPlayerByClient(client);
    auto objects = player.getMutableInventory().getVectorInventory();
    auto equip = player.getMutableEquipment().getVectorEquipment();
    objects.insert(objects.end(), equip.begin(), equip.end());

    if (containsKeyword(objects, keyword)) {
        auto item = getItemByKeyword(objects, keyword);
        auto roomId = accountHandler.getRoomIdByClient(client);
        playerHandler.dropItem(player, item);
        worldHandler.addItem(roomId, item);
        tempMessage << "Item dropped successfully.\n";
    } else {
        tempMessage << "Invalid keyword.\n";
    }
    return tempMessage.str();

}

std::string CommandExecutor::wear(const Connection &client, const std::string &keyword) {
    std::ostringstream tempMessage;
    auto &player = accountHandler.getPlayerByClient(client);
    auto objects = player.getMutableInventory().getVectorInventory();

    if (containsKeyword(objects, keyword)) {
        if (playerHandler.equipItem(player, getItemByKeyword(objects, keyword))) {
            tempMessage << "Item equipped successfully.\n";
        } else {
            tempMessage << "That item cannot be equipped!\n";
        }
    } else {
        tempMessage << "Invalid keyword.\n";
    }
    return tempMessage.str();
}

std::string CommandExecutor::remove(const Connection &client, const std::string &keyword) {
    std::ostringstream tempMessage;
    auto &player = accountHandler.getPlayerByClient(client);
    auto objects = player.getMutableEquipment().getVectorEquipment();

    if (containsKeyword(objects, keyword)) {
        playerHandler.unequipItem(player, getItemByKeyword(objects, keyword));
        tempMessage << "Item unequipped successfully.\n";
    } else {
        tempMessage << "Invalid keyword.\n";
    }
    return tempMessage.str();
}

std::vector<Message> CommandExecutor::give(const Connection &client, const std::string &username,
                                           const std::string &keyword) {
    std::vector<Message> messages = {};
    auto roomId = this->accountHandler.getRoomIdByClient(client);
    auto receiverClient = this->accountHandler.getClientByUsername(username);
    auto receiverId = this->accountHandler.getPlayerIdByClient(receiverClient);

    auto &sender = this->accountHandler.getPlayerByClient(client);
    auto senderName = this->accountHandler.getUsernameByClient(client);
    auto objects = sender.getMutableInventory().getVectorInventory();
    auto equip = sender.getMutableEquipment().getVectorEquipment();
    objects.insert(objects.end(), equip.begin(), equip.end());

    if (!this->worldHandler.canGive(roomId, receiverId) || username == senderName) {
        std::string message = "Invalid username.\n";
        messages.push_back({client, message});
    } else if (containsKeyword(objects, keyword)) {
        auto &receiver = this->accountHandler.getPlayerByClient(receiverClient);
        auto object = getItemByKeyword(objects, keyword);
        this->playerHandler.giveItem(sender, receiver, object);

        std::ostringstream senderMessage;
        std::ostringstream receiverMessage;

        senderMessage << "Item given successfully.\n";
        receiverMessage << senderName << " has given you " << object.getShortDescription() << std::endl;

        messages.push_back({client, senderMessage.str()});
        messages.push_back({receiverClient, receiverMessage.str()});

        return messages;
    } else {
        std::string message = "Invalid keyword.\n";
        messages.push_back({client, message});
    }

    return messages;
}

std::string CommandExecutor::status(const Connection &client) {
    std::ostringstream output;
    auto &player = accountHandler.getPlayerByClient(client);

    output << "\n"
           << "Status:\n"
           << "-------\n"
           << "Username: " << player.getUsername() << "\n";
    if (!this->magicHandler.isBodySwapped(client)) {
        switch (player.getRole()) {
            case Role::Admin:
                output << "Role: Admin\n";
                break;
            case Role::Builder:
                output << "Role: Builder\n";
                break;
            default:
                break;
        }
    }
    output << "HP: " << player.getHealth() << "/" << model::Character::STARTING_HEALTH << "\n";

    auto offenceValue = player.getMutableEquipment().getOffenceValue();
    auto minDamage = CombatHandler::BASE_MIN_DAMAGE + offenceValue;
    auto maxDamage = CombatHandler::BASE_MAX_DAMAGE + offenceValue;
    output << "Attack: " << minDamage << "-" << maxDamage << "\n";

    auto defenceValue = player.getMutableEquipment().getDefenceValue();
    output << "Armour: " << defenceValue << "\n";

    auto isBodySwapped = magicHandler.isBodySwapped(client);
    auto isConfused = magicHandler.isConfused(client);

    if (isBodySwapped) {
        output << "Body Swapped (This is not your body. How uncomfortable.)\n";
    }

    if (isConfused) {
        output << "Confused (No matter how hard you try, everything you say comes out all funny.)\n";
    }

    return output.str();
}

std::string CommandExecutor::alias(const Connection &client, const std::vector<std::string> &params) {
    if (params.empty()) {
        return "\nIncorrect number of parameters for alias command\n";
    }

    std::ostringstream res;

    std::string aliasOption = params[0];

    if (aliasOption == ALIAS_LIST) {
        res << aliasList(client, params);
    } else if (aliasOption == ALIAS_SET) {
        res << aliasSet(client, params);
    } else if (aliasOption == ALIAS_SET_GLOBAL) {
        res << aliasSetGlobal(client, params);
    } else if (aliasOption == ALIAS_CLEAR) {
        res << aliasClear(client, params);
    } else if (aliasOption == ALIAS_CLEAR_GLOBAL) {
        res << aliasClearGlobal(client, params);
    } else if (aliasOption.empty() || aliasOption == ALIAS_HELP) {
        res << aliasHelp();
    } else {
        res << aliasOption << " is not a valid option for "
            << this->commandParser.getStringForCommand(Command::Alias) << std::endl;
    }

    return res.str();
}

std::string CommandExecutor::aliasList(const Connection &client, const std::vector<std::string> &params) {
    std::string username = this->accountHandler.getUsernameByClient(client);

    if (params.size() != ALIAS_LIST_NUM_PARAMS) {
        return "\nIncorrect number of parameters for alias list command\n";
    }

    std::ostringstream res;

    auto aliases = this->aliasManager.getAliasesForUser(username);
    auto globalAliases = this->aliasManager.getGlobalAliases();

    res << "\nUser Aliases: \n";

    if (aliases.empty()) {
        res << "\tno user aliases set\n";
    }

    for (const auto &alias: aliases) {
        res << "\t" << alias.first << " -> " << alias.second << std::endl;
    }

    res << "Global Aliases: \n";

    for (const auto &alias: globalAliases) {
        res << "\t" << alias.first << " -> " << alias.second << std::endl;
    }

    if (globalAliases.empty()) {
        res << "\tno global aliases set\n";
    }

    return res.str();
}

std::string CommandExecutor::aliasSet(const Connection &client, const std::vector<std::string> &params) {
    if (params.size() != ALIAS_SET_NUM_PARAMS) {
        return "\nIncorrect number of parameters for alias set command\n";
    }

    std::ostringstream res;

    std::string username = this->accountHandler.getUsernameByClient(client);
    std::string command_to_alias_str = params[1];
    Command command_to_alias = this->commandParser.parseCommand(command_to_alias_str);

    if (command_to_alias == Command::InvalidCommand) {
        res << "\nAlias could not be set: " << command_to_alias_str
            << " is not the name of a command\n";
        return res.str();
    }

    std::string alias = params[2];

    if (!this->aliasManager.isValidAlias(alias)) {
        res << "\nAlias could not be set: " << alias << " is the name of a default command\n";
        return res.str();
    }

    if (this->aliasManager.setUserAlias(command_to_alias, alias, username)) {
        res << "\nAlias set successfully\n";
    } else {
        res << "\nAlias could not be set: an alias has already been set for the specified command\n";
    }

    return res.str();
}

std::string CommandExecutor::aliasSetGlobal(const Connection &client, const std::vector<std::string> &params) {
    if (this->magicHandler.isBodySwapped(client)) {
        return "\nYou cannot set global aliases while body swapped\n";
    }

    if (this->accountHandler.getPlayerByClient(client).getRole() != model::Role::Admin) {
        return "\nYou do not have permission to set global aliases\n";
    }

    if (params.size() != ALIAS_SET_NUM_PARAMS) {
        return "\nIncorrect number of parameters for alias set-global command\n";
    }

    std::ostringstream res;

    std::string command_to_alias_str = params[1];
    Command command_to_alias = this->commandParser.parseCommand(command_to_alias_str);

    if (command_to_alias == Command::InvalidCommand) {
        res << "\nAlias could not be set: " << command_to_alias_str
            << " is not the name of a command\n";
        return res.str();
    }

    std::string alias = params[2];

    if (!this->aliasManager.isValidAlias(alias)) {
        res << "\nAlias could not be set: " << alias << " is the name of a default command\n";
        return res.str();
    }

    if (this->aliasManager.setGlobalAlias(command_to_alias, alias)) {
        res << "\nAlias set successfully\n";
    } else {
        res << "\nAlias could not be set: an alias has already been set for the specified command\n";
    }

    return res.str();
}

std::string CommandExecutor::aliasClear(const Connection &client, const std::vector<std::string> &params) {
    if (params.size() != ALIAS_CLEAR_NUM_PARAMS) {
        return "\nIncorrect number of parameters for alias clear command\n";
    }

    std::ostringstream res;

    std::string command_to_clear_str = params[1];
    Command command_to_clear = this->commandParser.parseCommand(command_to_clear_str);

    if (command_to_clear == Command::InvalidCommand) {
        res << "\nAlias could not be cleared: " << command_to_clear_str
            << " is not the name of a command\n";
        return res.str();
    }

    std::string username = accountHandler.getUsernameByClient(client);
    this->aliasManager.clearUserAlias(command_to_clear, username);

    res << "\nAlias cleared successfully\n";
    return res.str();
}

std::string CommandExecutor::aliasClearGlobal(const Connection &client, const std::vector<std::string> &params) {
    if (this->magicHandler.isBodySwapped(client)) {
        return "\nYou cannot clear global aliases while body swapped\n";
    }

    if (accountHandler.getPlayerByClient(client).getRole() != model::Role::Admin) {
        return "\nYou do not have permission to clear global aliases\n";
    }

    if (params.size() != ALIAS_CLEAR_NUM_PARAMS) {
        return "\nIncorrect number of parameters for alias clear command\n";
    }

    std::string command_to_clear_str = params[1];
    Command command_to_clear = this->commandParser.parseCommand(command_to_clear_str);
    std::ostringstream res;

    if (command_to_clear == Command::InvalidCommand) {
        res << "\nAlias could not be cleared: " << command_to_clear_str
            << " is not the name of a command\n";
        return res.str();
    }

    this->aliasManager.clearGlobalAlias(command_to_clear);

    res << "\nAlias cleared successfully\n";
    return res.str();
}

std::string CommandExecutor::aliasHelp() {
    std::ostringstream res;
    res << "\nAlias Commands:\n"
        << "---------------\n"
        << "  - alias list (lists all aliases)\n"
        << "  - alias set [command] [alias] (sets an alias for a command)\n"
        << "  - alias set-global [command] [alias] (sets an alias that will be available to all users)\n"
        << "  - alias clear [command] (clears an alias for a command)\n"
        << "  - alias clear-global [command] (clears a global alias for a command)\n";
    return res.str();
}

std::string CommandExecutor::build() {
    std::ostringstream tempMessage;
    tempMessage << "\nWorld Building Commands:\n"
                << "------------------------\n"
                << "  - " << this->commandParser.getStringForCommand(Command::Acreate)
                << " [name] (creates an area with specified name)\n"
                << "  - " << this->commandParser.getStringForCommand(Command::Rcreate)
                << " [anum] [id] [name] (creates room in specified area, id cannot exceed "
                << MAX_ID_DIGITS << " digits)\n"
                << "  - " << this->commandParser.getStringForCommand(Command::Ocreate)
                << "/" << this->commandParser.getStringForCommand(Command::Ncreate)
                << " [id] [short description] (creates object/npc in current area, id cannot exceed "
                << MAX_ID_DIGITS << " digits)\n"
                << "  - " << this->commandParser.getStringForCommand(Command::Aedit)
                << " [field] [values] (modifies current area)\n"
                << "  - " << this->commandParser.getStringForCommand(Command::Redit)
                << " [field] [values] (modifies current room)\n"
                << "  - " << this->commandParser.getStringForCommand(Command::Oedit)
                << "/" << this->commandParser.getStringForCommand(Command::Nedit)
                << " [id] [field] [values] (modifies object/npc with specified id in current area)\n"
                << "  - " << this->commandParser.getStringForCommand(Command::Oreset)
                << " [id] (creates object reset for current room)\n"
                << "  - " << this->commandParser.getStringForCommand(Command::Nreset)
                << " [id] [amount] (creates NPC reset for current room, amount cannot exceed "
                << MAX_NPC_OF_TYPE_COUNT << ")\n"
                << "  - " << this->commandParser.getStringForCommand(Command::Alist)
                << " (lists world areas)\n"
                << "  - " << this->commandParser.getStringForCommand(Command::Rlist)
                << "/" << this->commandParser.getStringForCommand(Command::Olist)
                << "/" << this->commandParser.getStringForCommand(Command::Nlist)
                << " [anum] (lists rooms/objects/npcs of specified area)\n"
                << "  - " << this->commandParser.getStringForCommand(Command::Ashow)
                << " (shows current area name and resets)\n"
                << "  - " << this->commandParser.getStringForCommand(Command::Rshow)
                << " (shows current room state)\n"
                << "  - " << this->commandParser.getStringForCommand(Command::Oshow)
                << "/" << this->commandParser.getStringForCommand(Command::Nshow)
                << " [id] (shows state of object/npc in current area)\n"
                << "  - " << this->commandParser.getStringForCommand(Command::Goto)
                << " [id] (moves you to room with id)\n"
                << "  - " << this->commandParser.getStringForCommand(Command::Clear)
                << " (clears current room data including associated resets)\n"
                << "  - " << this->commandParser.getStringForCommand(Command::Reset)
                << " (resets current area with changes)\n";
    return tempMessage.str();
}

std::string CommandExecutor::builder(const std::string &username) {
    std::ostringstream tempMessage;
    for (const auto &connection: connectionHandler.getClients()) {
        auto name = this->accountHandler.getUsernameByClient(connection);
        if (username == name) {
            auto &player = this->accountHandler.getPlayerByClient(connection);
            auto role = player.getRole();
            if (role == model::Role::Default) {
                player.setRole(model::Role::Builder);
                tempMessage << "Access to build commands has been given to " << username << ".\n";
            } else if (role == model::Role::Builder) {
                player.setRole(model::Role::Default);
                tempMessage << "Access to build commands has been taken from " << username << ".\n";
            } else {
                tempMessage << "This command does not work on admin users.\n";
            }
            return tempMessage.str();
        }
    }
    tempMessage << "Unable to find online user \"" << username << "\".\n";
    return tempMessage.str();
}

std::string CommandExecutor::alist() {
    std::ostringstream tempMessage;
    tempMessage << "\nArea List\n";
    tempMessage << "---------\n";
    auto areas = this->worldHandler.getWorld().getAreas();
    int count = 1;
    for (const auto &area : areas) {
        tempMessage << "- " << count << ". " << area.getName() << std::endl;
        count++;
    }
    return tempMessage.str();
}

std::string CommandExecutor::rlist(const std::string &param) {
    std::ostringstream tempMessage;
    if (!param.empty() && std::all_of(param.begin(), param.end(), ::isdigit)) {
        auto index = static_cast<unsigned int>(std::stoi(param) - 1);
        if (index < this->worldHandler.getWorld().getAreas().size()) {
            tempMessage << "\nRoom List\n";
            tempMessage << "---------\n";
            auto area = this->worldHandler.getWorld().getAreas().at(index);
            for (const auto &room : area.getRooms()) {
                tempMessage << "- " << room.getId() << ". " << room.getName() << std::endl;
            }
        } else {
            tempMessage << "Invalid index.\n";
        }

    } else {
        tempMessage << "Invalid format.\n";
    }
    return tempMessage.str();
}

std::string CommandExecutor::olist(const std::string &param) {
    std::ostringstream tempMessage;
    if (!param.empty() && std::all_of(param.begin(), param.end(), ::isdigit)) {
        auto index = static_cast<unsigned int>(std::stoi(param) - 1);
        if (index < this->worldHandler.getWorld().getAreas().size()) {
            tempMessage << "\nObject List\n";
            tempMessage << "-----------\n";
            auto area = this->worldHandler.getWorld().getAreas().at(index);
            for (const auto &obj : area.getObjects()) {
                tempMessage << "- " << obj.getId() << ". " << obj.getShortDescription() << std::endl;
            }
        } else {
            tempMessage << "Invalid index.\n";
        }

    } else {
        tempMessage << "Invalid format.\n";
    }
    return tempMessage.str();
}

std::string CommandExecutor::nlist(const std::string &param) {
    std::ostringstream tempMessage;
    if (!param.empty() && std::all_of(param.begin(), param.end(), ::isdigit)) {
        auto index = static_cast<unsigned int>(std::stoi(param) - 1);
        if (index < this->worldHandler.getWorld().getAreas().size()) {
            tempMessage << "\nNPC List\n";
            tempMessage << "--------\n";
            auto area = this->worldHandler.getWorld().getAreas().at(index);
            for (const auto &npc : area.getNpcs()) {
                tempMessage << "- " << npc.getId() << ". " << npc.getShortDescription() << std::endl;
            }
        } else {
            tempMessage << "Invalid index.\n";
        }

    } else {
        tempMessage << "Invalid format.\n";
    }
    return tempMessage.str();
}

std::string CommandExecutor::oshow(const Connection &client, const std::string &param) {
    std::ostringstream tempMessage;
    if (!param.empty() && std::all_of(param.begin(), param.end(), ::isdigit)) {
        try {
            std::stoi(param);
        } catch (const std::out_of_range &exception) {
            tempMessage << "Invalid id.\n";
            return tempMessage.str();
        }
        int id = std::stoi(param);
        auto roomId = this->accountHandler.getRoomIdByClient(client);
        auto &area = this->worldHandler.findArea(roomId);
        if (area.objectExists(id)) {
            auto obj = area.findObjectById(id);
            tempMessage << *obj;
        } else {
            tempMessage << "Invalid id.\n";
        }
    } else {
        tempMessage << "Invalid format.\n";
    }
    return tempMessage.str();
}

std::string CommandExecutor::nshow(const Connection &client, const std::string &param) {
    std::ostringstream tempMessage;
    if (!param.empty() && std::all_of(param.begin(), param.end(), ::isdigit)) {
        try {
            std::stoi(param);
        } catch (const std::out_of_range &exception) {
            tempMessage << "Invalid id.\n";
            return tempMessage.str();
        }
        int id = std::stoi(param);
        auto roomId = this->accountHandler.getRoomIdByClient(client);
        auto &area = this->worldHandler.findArea(roomId);
        if (area.npcExists(id)) {
            auto npc = area.findNpcById(id);
            tempMessage << *npc;
        } else {
            tempMessage << "Invalid id.\n";
        }
    } else {
        tempMessage << "Invalid format.\n";
    }
    return tempMessage.str();
}

std::string CommandExecutor::go(const Connection &client, const std::string &param) {
    std::ostringstream tempMessage;
    if (!param.empty() && std::all_of(param.begin(), param.end(), ::isdigit)) {
        try {
            std::stoi(param);
        } catch (const std::out_of_range &exception) {
            tempMessage << "Invalid id.\n";
            return tempMessage.str();
        }
        int destinationId = std::stoi(param);
        auto roomId = this->accountHandler.getRoomIdByClient(client);
        if (destinationId == roomId) {
            tempMessage << "You are already in room " << destinationId << ".\n";

        } else if (this->worldHandler.roomExists(destinationId)) {
            auto playerId = this->accountHandler.getPlayerIdByClient(client);
            this->worldHandler.movePlayer(playerId, roomId, destinationId);
            this->accountHandler.setRoomIdByClient(client, destinationId);
            if (this->combatHandler.isInCombat(playerId)) {
                this->combatHandler.exitCombat(playerId);
                tempMessage << "You exit out of combat.\n";
            }

            tempMessage << "You are now in room " << destinationId << ".\n";

        } else {
            tempMessage << "Invalid id.\n";
        }

    } else {
        tempMessage << "Invalid format.\n";
    }

    return tempMessage.str();
}

template<typename T>
bool CommandExecutor::containsKeyword(const std::vector<T> &objects, const std::string &param) {
    auto keyword = boost::algorithm::to_lower_copy(param);
    auto it = std::find_if(objects.begin(), objects.end(),
                           [&keyword](const T &obj) { return obj.containsKeyword(keyword); });
    return (it != objects.end());
}

template<typename T>
T CommandExecutor::getItemByKeyword(const std::vector<T> &objects, const std::string &param) {
    auto keyword = boost::algorithm::to_lower_copy(param);
    T item;
    auto it = std::find_if(objects.begin(), objects.end(),
                           [&keyword](const T &obj) { return obj.containsKeyword(keyword); });
    if (it != objects.end()) {
        item = *it;
    }
    return item;
}
