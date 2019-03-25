//
// Created by louis on 19/01/19.
//

#include "Game.h"

#include <map>
#include <random>
#include <sstream>
#include <iostream>
#include <boost/algorithm/string.hpp>

using game::Game;

std::string
lowercase(std::string string) {
    std::transform(string.begin(), string.end(), string.begin(), ::tolower);
    return string;
}

std::string
trimWhitespace(const std::string &string) {
    std::string whitespace = " \t";
    auto start = string.find_first_not_of(whitespace);

    if (start == std::string::npos) {
        return "";
    }

    auto end = string.find_last_not_of(whitespace);
    auto size = end - start + 1;

    return string.substr(start, size);
}

namespace game {
    // move these when refactoring out commands
    constexpr auto ALIAS_LIST = "list";
    constexpr auto ALIAS_SET = "set";
    constexpr auto ALIAS_SET_GLOBAL = "set-global";
    constexpr auto ALIAS_CLEAR = "clear";
    constexpr auto ALIAS_CLEAR_GLOBAL = "clear-global";
    constexpr auto ALIAS_HELP = "help";
    constexpr auto ALIAS_SET_NUM_PARAMS = 3;
    constexpr auto ALIAS_CLEAR_NUM_PARAMS = 2;
    constexpr auto ALIAS_LIST_NUM_PARAMS = 1;

    Game::Game(std::vector<Connection> &clients,
               std::vector<Connection> &newClients,
               std::vector<Connection> &disconnectedClients,
               std::function<void(Connection)> &disconnect,
               std::function<void()> &shutdown) :
        clients(&clients),
        newClients(&newClients),
        disconnectedClients(&disconnectedClients),
        disconnect(disconnect),
        shutdown(shutdown),
        magicHandler(this->accountHandler),
        combatHandler(this->accountHandler, this->worldHandler){}

    void
    Game::handleConnects(std::deque<Message> &messages) {
        for (auto &newClient : *this->newClients) {
            std::ostringstream introduction;

            introduction << "Welcome to Adventure 2019!\n"
                         << "\n"
                         << "Enter " << "\"" << this->commandParser.getStringForCommand(Command::Login) << "\" to login to an existing account\n"
                         << "Enter " << "\"" << this->commandParser.getStringForCommand(Command::Register) << "\" to create a new account\n";

            messages.push_back({newClient, introduction.str()});
        }

        this->newClients->clear();
    }


    void
    Game::handleDisconnects(std::deque<Message> &messages) {
        for (auto &disconnectedClient : *this->disconnectedClients) {
            if (this->accountHandler.isLoggingIn(disconnectedClient)) {
                this->accountHandler.exitLogin(disconnectedClient);
                std::cout << disconnectedClient.id << " has been removed from login due to disconnect\n";
            }

            if (this->accountHandler.isRegistering(disconnectedClient)) {
                this->accountHandler.exitRegistration(disconnectedClient);
                std::cout << disconnectedClient.id << " has been removed from registration due to disconnect\n";
            }

            if (this->accountHandler.isLoggedIn(disconnectedClient)) {
                this->combatHandler.handleLogout(disconnectedClient);
                this->magicHandler.handleLogout(disconnectedClient);
                this->removeClientFromGame(disconnectedClient);
                this->accountHandler.logoutClient(disconnectedClient);
                std::cout << disconnectedClient.id << " has been logged out of the game due to disconnect\n";
            }
        }

        this->disconnectedClients->clear();
    }


    void
    Game::handleIncoming(const std::deque<Message> &incoming, std::deque<Message> &messages) {
        for (const auto &input : incoming) {
            auto client = input.connection;
            auto incomingInput = trimWhitespace(input.text);
            std::ostringstream tempMessage;

            if (this->accountHandler.isLoggingIn(client)) {
                messages.push_back({
                    client,
                    this->accountHandler.processLogin(client, incomingInput.substr(0, incomingInput.find(' ')))
                });

                if (this->accountHandler.isLoggedIn(client)) {
                    this->addClientToGame(client);
                    auto roomId = this->accountHandler.getRoomIdByClient(client);
                    tempMessage << "\n" << this->worldHandler.findRoom(roomId).descToString();
                    messages.push_back({client, tempMessage.str()});
                }

                continue;

            } else if (this->accountHandler.isRegistering(client)) {
                messages.push_back({
                    client,
                    this->accountHandler.processRegistration(client, incomingInput.substr(0, incomingInput.find(' ')))
                });

                if (this->accountHandler.isLoggedIn(client)) {
                    this->addClientToGame(client);
                    auto roomId = this->accountHandler.getRoomIdByClient(client);
                    tempMessage << "\n" << this->worldHandler.findRoom(roomId).descToString();
                    messages.push_back({client, tempMessage.str()});
                }

                continue;
            }

            std::string commandString = lowercase(incomingInput.substr(0, incomingInput.find(' ')));
            std::string username = this->accountHandler.getUsernameByClient(client);
            Command command = this->aliasManager.getCommandForUser(commandString, username);

            if (command == Command::InvalidCommand) {
                tempMessage << "The word \"" << commandString << "\" is not a valid command.\n";
                messages.push_back({client, tempMessage.str()});
                continue;
            }

            std::string parameters;

            if (incomingInput.find(' ') != std::string::npos) {
                parameters = trimWhitespace(incomingInput.substr(incomingInput.find(' ') + 1));
            }

            switch (command) {
                case Command::Quit: {
                    this->disconnect(input.connection);
                    continue;
                }

                case Command::Shutdown: {
                    std::cout << "Shutting down.\n";
                    this->shutdown();
                    return;
                }

                default:
                    break;
            }

            if (!this->accountHandler.isLoggedIn(client)) {
                messages.push_back(this->executeMenuAction(client, command, parameters));
            } else {
                if (this->isInvalidFormat(command, parameters)) {
                    tempMessage << "Invalid format for command \"" << commandString << "\".\n";
                    messages.push_back({client, tempMessage.str()});
                    continue;
                }
                auto responseList = this->executeInGameAction(client, command, parameters);

                for (auto &response : responseList) {
                    messages.push_back(response);
                }
            }
        }
    }

    Message
    Game::executeMenuAction(const Connection &client,
                            const Command &command,
                            const std::string &param) {
        std::ostringstream tempMessage;

        switch (command) {
            case Command::Register:
                tempMessage << this->accountHandler.processRegistration(client);
                break;

            case Command::Login:
                tempMessage << this->accountHandler.processLogin(client);
                break;

            case Command::Help:
                tempMessage << "\n"
                            << "********\n"
                            << "* HELP *\n"
                            << "********\n"
                            << "\n"
                            << "COMMANDS:\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Help) << " (shows this help interface)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Register) << " (create a new account)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Login) << " (login to an existing account)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Quit) << " (disconnects you from the game server)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Shutdown) << " (shuts down the game server)\n";
                break;

            default:
                tempMessage << "\n"
                            << "Enter " << "\"" << this->commandParser.getStringForCommand(Command::Login) << "\" to login to an existing account\n"
                            << "Enter " << "\"" << this->commandParser.getStringForCommand(Command::Register) << "\" to create a new account\n"
                            << "Enter " << "\"" << this->commandParser.getStringForCommand(Command::Help) << "\" for a full list of commands\n";
                break;
        }

        return {client, tempMessage.str()};
    }

    std::vector<Message>
    Game::executeInGameAction(const Connection &client,
                              const Command &command,
                              const std::string &param) {
        std::vector<Message> messages = {};
        std::ostringstream tempMessage;

        std::vector<std::string> params;
        boost::split(params, param, boost::is_any_of("\t "));

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
                            << "  - " << this->commandParser.getStringForCommand(Command::Help) << " (shows this help interface)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Chat) << " [message] (sends [message] to global chat)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Say) << " [message] (sends [message] to players in the same room)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Yell) << " [message] (sends [message] loud enough to be heard by players in adjacent rooms)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Tell) << " [username] [message] (sends [message] to [username] in the game)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Look) << " (displays current location information)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Exits) << " (displays exits from current location)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Move) << " [direction] (moves you in the direction specified)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Examine) << " [keyword] (examines something or someone)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Talk) << " [keyword] (interacts with NPC)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Attack) << " [keyword] (attack an NPC)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Flee) << " (attempt to escape from combat, moving to a random direction)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Take) << " [keyword] (places item in your inventory)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Drop) << " [keyword] (drops item from inventory/equipment)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Wear) << " [keyword] (equips item from your inventory)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Remove) << " [keyword] (unequips item to your inventory)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Give) << " [username] [keyword] (gives item to username)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Status) << " (displays your health, combat attributes, and current status ailments)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Inventory) << " (displays your inventory)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Equipment) << " (displays your equipment)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Spells) << " (displays available magic spells)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Cast) << " [spell] [target] (casts a spell on a target)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Alias) << " (aliases a command. Type \""
                                      << this->commandParser.getStringForCommand(Command::Alias) << " help\" for details)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Logout) << " (logs you out of the game)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Quit) << " (disconnects you from the game server)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Shutdown) << " (shuts down the game server)\n";
                break;

            case Command::Say: {
                auto roomId = this->accountHandler.getRoomIdByClient(client);
                auto playerIds = this->worldHandler.findRoom(roomId).getPlayersInRoom();

                for (const auto &playerId : playerIds) {
                    auto connection = this->accountHandler.getClientByPlayerId(playerId);
                    auto message = param;

                    if (this->magicHandler.isConfused(client)) {
                        this->magicHandler.confuseSpeech(message);
                    }

                    std::ostringstream sayMessage;
                    sayMessage << this->accountHandler.getUsernameByClient(client) << " says> " << message << "\n";

                    messages.push_back({connection, sayMessage.str()});
                }

                return messages;
            }

            case Command::Yell: {
                auto roomId = this->accountHandler.getRoomIdByClient(client);
                auto playerIds = this->worldHandler.getNearbyPlayerIds(roomId);

                for (const auto &playerId : playerIds) {
                    auto connection = this->accountHandler.getClientByPlayerId(playerId);
                    auto message = param;

                    if (this->magicHandler.isConfused(client)) {
                        this->magicHandler.confuseSpeech(message);
                    }

                    std::ostringstream sayMessage;
                    sayMessage << this->accountHandler.getUsernameByClient(client) << " yells> " << message << "\n";

                    messages.push_back({connection, sayMessage.str()});
                }

                return messages;
            }

            case Command::Tell: {
                auto username = param.substr(0, param.find(' '));
                auto message = trimWhitespace(param.substr(param.find(' ') + 1));

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

            case Command::Chat: {
                auto message = param;

                if (this->magicHandler.isConfused(client)) {
                    this->magicHandler.confuseSpeech(message);
                }

                for (auto connection : *this->clients) {
                    if (this->accountHandler.isLoggedIn(connection)) {
                        std::ostringstream chatMessage;
                        chatMessage << this->accountHandler.getUsernameByClient(client) << "> " << message << "\n";
                        messages.push_back({connection, chatMessage.str()});
                    }
                }

                return messages;
            }

            case Command::Look: {
                if (param.empty()) {
                    auto roomId = this->accountHandler.getRoomIdByClient(client);
                    auto room = this->worldHandler.findRoom(roomId);
                    tempMessage << room;
                    tempMessage << "[Players]\n";
                    for (const auto &id : room.getPlayersInRoom()) {
                        tempMessage << this->accountHandler.getUsernameByPlayerId(id) << std::endl;
                    }
                    break;
                }
            }

            case Command::Examine: {
                auto room = this->worldHandler.findRoom(this->accountHandler.getRoomIdByClient(client));
                auto objects = room.getObjects();
                auto npcs = room.getNpcs();
                auto extras = room.getExtras();

                if (containsKeyword(objects, param)) {
                    auto obj = getItemByKeyword(objects, param);
                    for (const auto &str : obj.getLongDescription()) {
                        tempMessage << str << std::endl;
                    }
                } else if (containsKeyword(npcs, param)) {
                    auto npc = getItemByKeyword(npcs, param);
                    for (const auto &str : npc.getDescription()) {
                        tempMessage << str << std::endl;
                    }
                } else if (containsKeyword(extras, param)) {
                    auto extra = getItemByKeyword(extras, param);
                    for (const auto &str : extra.getExtraDescriptions()) {
                        tempMessage << str << std::endl;
                    }
                } else {
                    tempMessage << "Invalid keyword.\n";
                }

                break;
            }

            case Command::Exits: {
                auto roomId = this->accountHandler.getRoomIdByClient(client);
                tempMessage << "\n" << this->worldHandler.findRoom(roomId).doorsToString();
                break;
            }

            case Command::Move: {
                auto roomId = this->accountHandler.getRoomIdByClient(client);
                auto dir = lowercase(param);

                if (this->worldHandler.isValidDirection(roomId, dir)) {
                    auto player = this->accountHandler.getPlayerByClient(client);

                    if (this->combatHandler.isInCombat(*player)) {
                        tempMessage << "You can't expect to just stroll out of here with someone attacking you!"
                                    << " Perhaps you should flee instead.\n";
                        break;
                    }

                    auto playerId = this->accountHandler.getPlayerIdByClient(client);
                    auto destinationId = this->worldHandler.getDestination(roomId, dir);
                    this->worldHandler.movePlayer(playerId, roomId, destinationId);
                    this->accountHandler.setRoomIdByClient(client, destinationId);
                    tempMessage << "\n" << this->worldHandler.findRoom(destinationId).descToString();

                } else {
                    tempMessage << "You can't move that way!\n";
                }

                break;
            }

            case Command::Cast: {
                return this->magicHandler.castSpell(client, param);
            }

            case Command::Spells: {
                tempMessage << this->magicHandler.getSpells();
                break;
            }

            case Command::Talk: {
                auto room = this->worldHandler.findRoom(this->accountHandler.getRoomIdByClient(client));
                auto npcs = room.getNpcs();

                if (!containsKeyword(npcs, param)) {
                    tempMessage << "Invalid keyword.\n";
                    break;
                }

                auto player = this->accountHandler.getPlayerByClient(client);
                auto npc = getItemByKeyword(npcs, param);

                if (this->combatHandler.isInCombat(*player)) {
                    tempMessage << "Less talking, more fighting.\n";
                    break;
                }

                if (this->combatHandler.isInCombat(npc) && !this->combatHandler.areInCombat(*player, npc)) {
                    auto otherPlayerId = this->combatHandler.getOpponentId(npc);
                    auto otherPlayerName = this->accountHandler.getUsernameByPlayerId(otherPlayerId);

                    tempMessage << npc.getShortDescription() << " is busy fighting " << otherPlayerName << "\n";
                    break;

                }

                for (const auto &str : npc.getLongDescription()) {
                    tempMessage << str << std::endl;
                }

                break;
            }

            case Command::Attack: {
                tempMessage << this->combatHandler.attack(client, param);
                break;
            }

            case Command::Flee: {
                tempMessage << this->combatHandler.flee(client);
                break;
            }

            case Command::Take: {
                auto roomId = this->accountHandler.getRoomIdByClient(client);
                Room room = this->worldHandler.findRoom(roomId);
                auto objects = room.getObjects();

                if (containsKeyword(objects, param)) {
                    auto item = getItemByKeyword(objects, param);
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

                if (containsKeyword(objects, param)) {
                    auto item = getItemByKeyword(objects, param);
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

                if (containsKeyword(objects, param)) {
                    if (this->playerHandler.equipItem(*player, getItemByKeyword(objects, param))) {
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

                if (containsKeyword(objects, param)) {
                    this->playerHandler.unequipItem(*player, getItemByKeyword(objects, param));
                    tempMessage << "Item unequipped successfully.\n";
                } else {
                    tempMessage << "Invalid keyword.\n";
                }

                break;
            }

            case Command::Give: {
                auto username = param.substr(0, param.find(' '));
                auto keyword = trimWhitespace(param.substr(param.find(' ') + 1));

                auto roomId = this->accountHandler.getRoomIdByClient(client);
                auto receiverClient = this->accountHandler.getClientByUsername(username);
                auto receiverId = this->accountHandler.getPlayerIdByClient(receiverClient);

                auto sender = this->accountHandler.getPlayerByClient(client);
                auto senderName = this->accountHandler.getUsernameByClient(client);
                auto objects = sender->getInventory().getVectorInventory();
                auto equip = sender->getEquipment().getVectorEquipment();
                objects.insert(objects.end(), equip.begin(), equip.end());

                if (!this->worldHandler.canGive(roomId, receiverId) || username == senderName) {
                    tempMessage << "Invalid username.\n";
                } else if (containsKeyword(objects, keyword)) {
                    auto receiver = this->accountHandler.getPlayerByClient(receiverClient);
                    auto object = getItemByKeyword(objects, keyword);
                    this->playerHandler.giveItem(*sender, *receiver, object);

                    std::ostringstream senderMessage;
                    std::ostringstream receiverMessage;

                    senderMessage << "Item given successfully.\n";
                    receiverMessage << senderName << " has given you " << object.getShortDescription() << std::endl;

                    messages.push_back({client, senderMessage.str()});
                    messages.push_back({receiverClient, receiverMessage.str()});

                    return messages;
                } else {
                    tempMessage << "Invalid keyword.\n";
                }

                break;
            }

            case Command::Status: {
                auto player = this->accountHandler.getPlayerByClient(client);

                tempMessage << "\n"
                            << "Status:\n"
                            << "-------\n"
                            << "HP: " << player->getHealth() << "/" << model::Character::STARTING_HEALTH << "\n";

                auto offenceValue = this->combatHandler.getEquipmentOffence(*player);
                auto minDamage = CombatHandler::BASE_MIN_DAMAGE + offenceValue;
                auto maxDamage = CombatHandler::BASE_MAX_DAMAGE + offenceValue;
                tempMessage << "Attack: " << minDamage << "-" << maxDamage << " HP\n";

                auto defenceValue = this->combatHandler.getEquipmentDefence(*player);
                tempMessage << "Armour: " << defenceValue << "\n";

                auto isBodySwapped = this->magicHandler.isBodySwapped(client);
                auto isConfused = this->magicHandler.isConfused(client);

                if (isBodySwapped) {
                    tempMessage << "Body Swapped (This is not your body. How uncomfortable.)\n";
                }

                if (isConfused) {
                    tempMessage << "Confused (No matter how hard you try, everything you say comes out all funny.)\n";
                }

                tempMessage << "\n";

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
                try {
                    std::string username = this->accountHandler.getUsernameByClient(client);
                    if (params.empty()) {
                        tempMessage << "\nIncorrect number of parameters for alias command\n";
                        break;
                    }
                    std::string aliasOption = params[0];

                    if (aliasOption == ALIAS_LIST) {
                        if (params.size() != ALIAS_LIST_NUM_PARAMS) {
                            tempMessage << "\nIncorrect number of parameters for alias list command\n";
                            break;
                        }
                        auto aliases = this->aliasManager.getAliasesForUser(username);
                        auto globalAliases = this->aliasManager.getGlobalAliases();

                        tempMessage << "\nUser Aliases: \n";
                        if (aliases.empty()) {
                            tempMessage << "\tno user aliases set\n";
                        }
                        for (const auto &alias: aliases) {
                            tempMessage << "\t" << alias.first << " -> " << alias.second << std::endl;
                        }

                        tempMessage << "Global Aliases: \n";
                        for (const auto &alias: globalAliases) {
                            tempMessage << "\t" << alias.first << " -> " << alias.second << std::endl;
                        }
                        if (globalAliases.empty()) {
                            tempMessage << "\tno global aliases set\n";
                        }
                    } else if (aliasOption == ALIAS_SET || aliasOption == ALIAS_SET_GLOBAL) {
                        if (params.size() != ALIAS_SET_NUM_PARAMS) {
                            tempMessage << "\nIncorrect number of parameters for alias set command\n";
                            break;
                        }

                        std::string command_to_alias_str = params[1];
                        Command command_to_alias = this->commandParser.parseCommand(command_to_alias_str);

                        if (command_to_alias == Command::InvalidCommand) {
                            tempMessage << "\nAlias could not be set: " << command_to_alias_str << " is not the name of a command\n";
                            break;
                        }

                        std::string alias = params[2];

                        if (!this->aliasManager.isValidAlias(alias)) {
                            tempMessage << "\nAlias could not be set: " << alias << " is the name of a default command\n";
                            break;
                        }

                        if ((aliasOption == ALIAS_SET &&
                             this->aliasManager.setUserAlias(command_to_alias, alias, username)) ||
                            (aliasOption == ALIAS_SET_GLOBAL &&
                             this->aliasManager.setGlobalAlias(command_to_alias, alias))) {
                            tempMessage << "\nAlias set successfully\n";
                        } else {
                            tempMessage << "\nAlias could not be set: an alias has already been set for the specified command\n";
                        }
                    } else if (aliasOption == ALIAS_CLEAR || aliasOption == ALIAS_CLEAR_GLOBAL) {
                        if (params.size() != ALIAS_CLEAR_NUM_PARAMS) {
                            tempMessage << "\nIncorrect number of parameters for alias clear command\n";
                            break;
                        }
                        std::string command_to_clear_str = params[1];
                        Command command_to_clear = this->commandParser.parseCommand(command_to_clear_str);

                        if (command_to_clear == Command::InvalidCommand) {
                            tempMessage << "\nAlias could not be cleared: " << command_to_clear_str << " is not the name of a command\n";
                            break;
                        }

                        if (aliasOption == ALIAS_CLEAR) {
                            this->aliasManager.clearUserAlias(command_to_clear, username);
                        } else {
                            this->aliasManager.clearGlobalAlias(command_to_clear);
                        }

                        tempMessage << "\nAlias cleared successfully\n";

                    } else if (aliasOption.empty() || aliasOption == ALIAS_HELP) {
                        tempMessage << "\nAlias Commands:\n"
                                    << "---------------\n"
                                    << "  - alias list (lists all aliases)\n"
                                    << "  - alias set [command] [alias] (sets an alias for a command)\n"
                                    << "  - alias set-global [command] [alias] (sets an alias that will be available to all users)\n"
                                    << "  - alias clear [command] (clears an alias for a command)\n"
                                    << "  - alias clear-global [command] (clears a global alias for a command)\n";
                    } else {
                        tempMessage << aliasOption << " is not a valid option for "
                                    << this->commandParser.getStringForCommand(Command::Alias) << std::endl;
                    }
                } catch (std::exception &e) {
                    tempMessage << "\n error parsing " << this->commandParser.getStringForCommand(Command::Alias)
                            << " command!\n";
                    std::cout << e.what();
                }

                break;
            }

            default:
                tempMessage << "\nEnter " << "\"" << this->commandParser.getStringForCommand(Command::Help) << "\" for a full list of commands\n";
                break;
        }

        messages.push_back({client, tempMessage.str()});

        return messages;
    }


    void
    Game::handleOutgoing(std::deque<Message> &messages) {
        this->accountHandler.notifyBootedClients(messages);
        this->magicHandler.processCycle(messages);
        this->combatHandler.processCycle(messages);
    }


    std::deque<Message>
    Game::formMessages(std::deque<Message> &messages) {
        std::deque<Message> outgoing;
        std::map<Connection, std::ostringstream> clientMessages;

        for (const auto &message : messages) {
            clientMessages[message.connection] << message.text;
        }

        for (auto const& [client, message] : clientMessages) {
            outgoing.push_back({client, message.str()});
        }

        return outgoing;
    }


    void
    Game::addClientToGame(Connection client) {
        auto playerId = this->accountHandler.getPlayerIdByClient(client);
        auto roomId = this->accountHandler.getRoomIdByClient(client);
        this->worldHandler.addPlayer(roomId, playerId);
    }


    void
    Game::removeClientFromGame(Connection client) {
        auto playerId = this->accountHandler.getPlayerIdByClient(client);
        auto roomId = this->accountHandler.getRoomIdByClient(client);
        this->worldHandler.removePlayer(roomId, playerId);
    }


    bool
    Game::isInvalidFormat(const Command &command, const std::string &parameters) {
        bool wrongFormat = ((command == Command::Tell || command == Command::Give)
            && parameters.find(' ') == std::string::npos);
        bool isCommandWithParam = (command == Command::Say
            || command == Command::Yell
            || command == Command::Move
            || command == Command::Examine
            || command == Command::Talk
            || command == Command::Take
            || command == Command::Drop
            || command == Command::Wear
            || command == Command::Remove);

        return (wrongFormat || (isCommandWithParam && parameters.empty()));
    }


    template <typename T>
    bool
    Game::containsKeyword(const std::vector<T> &objects, const std::string &param) {
        auto keyword = lowercase(param);
        auto it = std::find_if(objects.begin(), objects.end(), [&keyword](const T &obj) {return obj.containsKeyword(keyword);});
        return (it != objects.end());
    }


    template <typename T>
    T
    Game::getItemByKeyword(const std::vector<T> &objects, const std::string &param) {
        auto keyword = lowercase(param);
        T item;
        auto it = std::find_if(objects.begin(), objects.end(), [&keyword](const T &obj) {return obj.containsKeyword(keyword);});
        if (it != objects.end()) {
            item = *it;
        }
        return item;
    }


    std::deque<Message>
    Game::processCycle(std::deque<Message> &incoming) {
        std::deque<Message> messages;

        this->handleConnects(messages);
        this->handleDisconnects(messages);
        this->handleIncoming(incoming, messages);
        this->handleOutgoing(messages);

        return this->formMessages(messages);
    }
}
