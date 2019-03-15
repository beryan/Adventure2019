//
// Created by Stephen Wanhella on 3/14/2019.
//

#include <CommandExecutor.h>
#include <sstream>
#include <iostream>

#include "CommandExecutor.h"
#include "Room.h"
#include "NPC.h"

using game::CommandExecutor;
using model::NPC;
using model::Room;

constexpr auto ALIAS_LIST = "list";
constexpr auto ALIAS_SET = "set";
constexpr auto ALIAS_CLEAR = "clear";
constexpr auto ALIAS_HELP = "help";

std::string CommandExecutor::executeCommand(const Connection &client, const Command &command,
                                            const std::vector<std::string> &params) {
    std::ostringstream tempMessage;
    switch (command) {
        case Command::Logout: {
            this->magicHandler.handleLogout(client);
            this->removeClientFromGame(client);
            tempMessage << this->accountHandler.logoutClient(client);
            break;
        }

        case Command::Help:
            tempMessage << "\n"
                        << "********\n"
                        << "* HELP *\n"
                        << "********\n"
                        << "\n"
                        << "COMMANDS:\n"
                        << "  - " << this->commandParser.getStringForCommand(Command::Help)
                        << " (shows this help interface)\n"
                        << "  - " << this->commandParser.getStringForCommand(Command::Say)
                        << " [message] (sends [message] to nearby players in the game)\n"
                        << "  - " << this->commandParser.getStringForCommand(Command::Tell)
                        << " [username] [message] (sends [message] to [username] in the game)\n"
                        << "  - " << this->commandParser.getStringForCommand(Command::Yell)
                        << " [message] (sends [message] to other players in the game)\n"
                        << "  - " << this->commandParser.getStringForCommand(Command::Look)
                        << " (displays current location information)\n"
                        << "  - " << this->commandParser.getStringForCommand(Command::Exits)
                        << " (displays exits from current location)\n"
                        << "  - " << this->commandParser.getStringForCommand(Command::Move)
                        << " [direction] (moves you in the direction specified)\n"
                        << "  - " << this->commandParser.getStringForCommand(Command::Examine)
                        << " [keyword] (examines something or someone)\n"
                        << "  - " << this->commandParser.getStringForCommand(Command::Talk)
                        << " [keyword] (interacts with NPC)\n"
                        << "  - " << this->commandParser.getStringForCommand(Command::Take)
                        << " [keyword] (places item in your inventory)\n"
                        << "  - " << this->commandParser.getStringForCommand(Command::Drop)
                        << " [keyword] (drops item from inventory/equipment)\n"
                        << "  - " << this->commandParser.getStringForCommand(Command::Wear)
                        << " [keyword] (equips item from your inventory)\n"
                        << "  - " << this->commandParser.getStringForCommand(Command::Remove)
                        << " [keyword] (unequips item to your inventory)\n"
                        << "  - " << this->commandParser.getStringForCommand(Command::Inventory)
                        << " (displays your inventory)\n"
                        << "  - " << this->commandParser.getStringForCommand(Command::Equipment)
                        << " (displays your equipment)\n"
                        << "  - " << this->commandParser.getStringForCommand(Command::Spells)
                        << " (displays available magic spells)\n"
                        << "  - " << this->commandParser.getStringForCommand(Command::Cast)
                        << " [spell] [target] (casts a spell on a target)\n"
                        << "  - " << this->commandParser.getStringForCommand(Command::Logout)
                        << " (logs you out of the game)\n"
                        << "  - " << this->commandParser.getStringForCommand(Command::Quit)
                        << " (disconnects you from the game server)\n"
                        << "  - " << this->commandParser.getStringForCommand(Command::Shutdown)
                        << " (shuts down the game server)\n";
            break;

        case Command::Say: {
            auto roomId = this->accountHandler.getRoomIdByClient(client);
            auto playerIds = this->worldHandler.getNearbyPlayerIds(roomId);


            for (const auto &playerId : playerIds) {
                auto connection = this->accountHandler.getClientByPlayerId(playerId);
                auto message = !params.empty() ? params[0] : "";

                if (this->magicHandler.isConfused(client)) {
                    this->magicHandler.confuseSpeech(message);
                }

                std::ostringstream sayMessage;
                sayMessage << this->accountHandler.getUsernameByClient(client) << "> " << message << "\n";

                messages.push_back({connection, sayMessage.str()});
            }

            return messages;
        }

        case Command::Tell: {
            auto username = params[0];
            auto message = params[1];

            if (this->magicHandler.isConfused(client)) {
                this->magicHandler.confuseSpeech(message);
            }

            for (auto connection: *this->clients) {
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

            tempMessage << "Unable to find online user \"" << username << "\".\n";

            break;
        }

        case Command::Yell: {
            std::string message = !params.empty() ? params[0] : "";

            if (this->magicHandler.isConfused(client)) {
                this->magicHandler.confuseSpeech(message);
            }

            for (auto connection : *this->clients) {
                std::ostringstream yellMessage;
                yellMessage << this->accountHandler.getUsernameByClient(client) << "> " << message << "\n";
                messages.push_back({connection, yellMessage.str()});
            }

            return messages;
        }

        case Command::Look: {
            if (params.empty()) {
                auto roomId = this->accountHandler.getRoomIdByClient(client);
                tempMessage << this->worldHandler.findRoom(roomId);
                break;
            }
        }

        case Command::Examine: {
            auto room = this->worldHandler.findRoom(this->accountHandler.getRoomIdByClient(client));
            auto npcs = room.getNpcs();
            auto objects = room.getObjects();

            std::string keyword = !params.empty() ? params[0] : "";

            if (containsKeyword(npcs, keyword)) {
                NPC npc = getItemByKeyword(npcs, keyword);
                for (const auto &str : npc.getDescription()) {
                    tempMessage << str << std::endl;
                }
            } else if (containsKeyword(objects, keyword)) {
                Object obj = getItemByKeyword(objects, keyword);
                for (const auto &str : obj.getLongDescription()) {
                    tempMessage << str << std::endl;
                }
            } else {
                tempMessage << "Invalid keyword.\n";
            }

            break;
        }

        case Command::Exits: {
            auto roomID = this->accountHandler.getRoomIdByClient(client);
            tempMessage << "\n" << this->worldHandler.findRoom(roomID).doorsToString();
            break;
        }

        case Command::Move: {
            auto roomId = this->accountHandler.getRoomIdByClient(client);
            std::string direction = !params.empty() ? params[0] : "";
            if (this->worldHandler.isValidDirection(roomId, direction)) {
                auto playerId = this->accountHandler.getPlayerIdByClient(client);
                auto destinationId = this->worldHandler.getDestination(roomId, direction);
                this->worldHandler.movePlayer(playerId, roomId, destinationId);
                this->accountHandler.setRoomIdByClient(client, destinationId);
                tempMessage << "\n" << this->worldHandler.findRoom(destinationId).descToString();
            } else {
                tempMessage << "You can't move that way!\n";
            }

            break;
        }

        case Command::Cast: {
            return this->magicHandler.castSpell(client, params[0]);
        }

        case Command::Spells: {
            tempMessage << this->magicHandler.getSpells();
            break;
        }

        case Command::Talk: {
            auto room = this->worldHandler.findRoom(this->accountHandler.getRoomIdByClient(client));
            auto npcs = room.getNpcs();
            std::string keyword = !params.empty() ? params[0] : "";
            if (containsKeyword(npcs, keyword)) {
                auto npc = getItemByKeyword(npcs, keyword);
                for (const auto &str : npc.getLongDescription()) {
                    tempMessage << str << std::endl;
                }
            } else {
                tempMessage << "Invalid keyword.\n";
            }

            break;
        }

        case Command::Take: {
            auto roomId = this->accountHandler.getRoomIdByClient(client);
            Room &room = this->worldHandler.findRoom(roomId);
            auto objects = room.getObjects();

            if (containsKeyword(objects, params[0])) {
                auto item = getItemByKeyword(objects, params[0]);
                auto player = this->accountHandler.getPlayerByClient(client);
                this->worldHandler.removeItem(roomId, item.getId());
                this->playerHandler.pickupItem(*player, item);
                tempMessage << "Item taken successfully.\n";
            } else {
                tempMessage << "Invalid keyword.\n";
            }

            break;
        }

        case Command::Drop: {
            auto player = this->accountHandler.getPlayerByClient(client);
            auto objects = player->getInventory().getVectorInventory();
            auto equip = player->getEquipment().getVectorEquipment();
            objects.insert(objects.end(), equip.begin(), equip.end());

            if (containsKeyword(objects, params[0])) {
                auto item = getItemByKeyword(objects, params[0]);
                auto roomId = this->accountHandler.getRoomIdByClient(client);

                this->playerHandler.dropItem(*player, item);
                this->worldHandler.addItem(roomId, item);
                tempMessage << "Item dropped successfully.\n";
            } else {
                tempMessage << "Invalid keyword.\n";
            }

            break;
        }

        case Command::Wear: {
            auto player = this->accountHandler.getPlayerByClient(client);
            auto objects = player->getInventory().getVectorInventory();

            if (containsKeyword(objects, params[0])) {
                if (this->playerHandler.equipItem(*player, getItemByKeyword(objects, params[0]))) {
                    tempMessage << "Item equipped successfully.\n";
                } else {
                    tempMessage << "That item cannot be equipped!\n";
                }
            } else {
                tempMessage << "Invalid keyword.\n";
            }

            break;
        }

        case Command::Remove: {
            auto player = this->accountHandler.getPlayerByClient(client);
            auto objects = player->getEquipment().getVectorEquipment();

            if (containsKeyword(objects, params[0])) {
                this->playerHandler.unequipItem(*player, getItemByKeyword(objects, params[0]));
                tempMessage << "Item unequipped successfully.\n";
            } else {
                tempMessage << "Invalid keyword.\n";
            }

            break;
        }

        case Command::Inventory: {
            tempMessage << this->accountHandler.getPlayerByClient(client)->getInventory();
            break;
        }

        case Command::Equipment: {
            tempMessage << this->accountHandler.getPlayerByClient(client)->getEquipment();
            break;
        }

        case Command::Debug: {
            tempMessage << this->worldHandler.getWorld();
            break;
        }

        case Command::Alias: {
            tempMessage << executeAliasCommand(client, params);
            break;
        }

        default:
            tempMessage << "\nEnter " << "\"" << this->commandParser.getStringForCommand(Command::Help)
                        << "\" for a full list of commands\n";
            break;
    }
}

std::string CommandExecutor::executeAliasCommand(const Connection &client, const std::vector<std::string> &params) {
    std::ostringstream res;
    try {
        std::string username = accountHandler.getUsernameByClient(client);
        std::string aliasOption = params[0];

        if (aliasOption == ALIAS_LIST) {
            auto aliases = aliasManager.getAliasesForUser(username);
            auto globalAliases = aliasManager.getGlobalAliases();

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
        } else if (aliasOption == ALIAS_SET) {
            std::string command_to_alias_str = params[1];
            Command command_to_alias = commandParser.parseCommand(command_to_alias_str);
            if (command_to_alias != Command::InvalidCommand) {
                std::string alias = params[2];
                if (aliasManager.isValidAlias(alias)) {
                    if (aliasManager.setUserAlias(command_to_alias, alias, username)) {
                        res << "\nalias set successfully\n";
                    } else {
                        res << "\nalias could not be set\n";
                    }
                } else {
                    res << std::endl << alias << " is not a valid alias\n";
                }
            } else {
                res << std::endl << command_to_alias_str << " did not map to a command\n";
            }
        } else if (aliasOption == ALIAS_CLEAR) {
            std::string command_to_clear_str = params[1];
            Command command_to_clear = commandParser.parseCommand(command_to_clear_str);
            if (command_to_clear != Command::InvalidCommand) {
                aliasManager.clearUserAlias(command_to_clear, username);
                res << "\nalias cleared successfully\n";
            } else {
                res << std::endl << command_to_clear_str << " did not map to a command\n";
            }
        } else if (aliasOption.empty() || aliasOption == ALIAS_HELP) {
            res << "\nalias help: \n";
            res << "\talias list: list all aliases\n";
            res << "\talias set [command to alias] [alias]: sets an alias\n";
            res << "\talias clear [aliased command]: clear an alias for a command\n";
        } else {
            res << aliasOption << " is not a valid option for "
                << commandParser.getStringForCommand(Command::Alias) << std::endl;
        }
    } catch (std::exception &e) {
        res << "\n error parsing " << commandParser.getStringForCommand(Command::Alias)
            << " command!\n";
        std::cout << e.what();
        return res.str();
    }
    return res.str();
}

template<typename T>
bool
CommandExecutor::containsKeyword(const std::vector<T> &objects, const std::string &keyword) {
    auto it = std::find_if(objects.begin(), objects.end(),
                           [&keyword](const T &obj) { return obj.containsKeyword(keyword); });
    return (it != objects.end());
}


template<typename T>
T
CommandExecutor::getItemByKeyword(const std::vector<T> &objects, const std::string &keyword) {
    T item;
    auto it = std::find_if(objects.begin(), objects.end(),
                           [&keyword](const T &obj) { return obj.containsKeyword(keyword); });
    if (it != objects.end()) {
        item = *it;
    }
    return item;
}