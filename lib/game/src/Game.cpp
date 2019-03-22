//
// Created by louis on 19/01/19.
//

#include "Game.h"

#include <map>
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
        magicHandler(this->accountHandler){};


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
                    auto roomID = this->accountHandler.getRoomIdByClient(client);
                    tempMessage << "\n" << this->worldHandler.findRoom(roomID).descToString();
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
                    auto roomID = this->accountHandler.getRoomIdByClient(client);
                    tempMessage << "\n" << this->worldHandler.findRoom(roomID).descToString();
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
                            << "  - " << this->commandParser.getStringForCommand(Command::Take) << " [keyword] (places item in your inventory)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Drop) << " [keyword] (drops item from inventory/equipment)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Wear) << " [keyword] (equips item from your inventory)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Remove) << " [keyword] (unequips item to your inventory)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Give) << " [username] [keyword] (gives item to username)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Inventory) << " (displays your inventory)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Equipment) << " (displays your equipment)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Spells) << " (displays available magic spells)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Cast) << " [spell] [target] (casts a spell on a target)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Build) << " (displays available world building commands)\n"
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
                    tempMessage << room << "Players:\n";;
                    for (const auto &id : room.getPlayersInRoom()) {
                        tempMessage << "- " << this->accountHandler.getUsernameByPlayerId(id) << std::endl;
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
                auto roomID = this->accountHandler.getRoomIdByClient(client);
                tempMessage << "\n" << this->worldHandler.findRoom(roomID).doorsToString();
                break;
            }

            case Command::Move: {
                auto roomId = this->accountHandler.getRoomIdByClient(client);
                auto dir = lowercase(param);

                if (this->worldHandler.isValidDirection(roomId, dir)) {
                    auto destinationId = this->worldHandler.getDestination(roomId, dir);
                    if (this->worldHandler.roomExists(destinationId)) {
                        auto playerId = this->accountHandler.getPlayerIdByClient(client);
                        this->worldHandler.movePlayer(playerId, roomId, destinationId);
                        this->accountHandler.setRoomIdByClient(client, destinationId);
                        tempMessage << "\n" << this->worldHandler.findRoom(destinationId).descToString();
                    } else {
                        tempMessage << "You tried to move to a room that does not exist!\n";
                    }
                } else {
                    tempMessage << "You can't move that way!\n";
                }

                break;
            }

            case Command::Goto: {
                if (std::all_of(param.begin(), param.end(), ::isdigit)) {
                    int destinationId = std::stoi(param);
                    auto roomId = this->accountHandler.getRoomIdByClient(client);
                    if (destinationId == roomId) {
                        tempMessage << "You are already in room " << param << ".\n";
                    } else if (this->worldHandler.roomExists(destinationId)) {
                        auto playerId = this->accountHandler.getPlayerIdByClient(client);
                        this->worldHandler.movePlayer(playerId, roomId, destinationId);
                        this->accountHandler.setRoomIdByClient(client, destinationId);
                        tempMessage << "You are now in room " << param << ".\n";
                    } else {
                        tempMessage << "That room does not exist.\n";
                    }
                } else {
                    tempMessage << "Invalid room id.\n";
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

                if (containsKeyword(npcs, param)) {
                    auto npc = getItemByKeyword(npcs, param);
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
                Room& room = this->worldHandler.findRoom(roomId);
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

            case Command::Inventory: {
                tempMessage << this->accountHandler.getPlayerByClient(client)->getInventory();
                break;
            }

            case Command::Equipment: {
                tempMessage << this->accountHandler.getPlayerByClient(client)->getEquipment();
                break;
            }

            case Command::Build: {
                tempMessage << "\n"
                            << "******************\n"
                            << "* World Building *\n"
                            << "******************\n"
                            << "\n"
                            << "COMMANDS:\n"
                            << "  - acreate [name] (creates an area with specified name)\n"
                            << "  - rcreate [anum] [id] [name] (creates room in specified area)\n"
                            << "  - ocreate/ncreate [anum] [id] [short description] (creates object/npc in specified area)\n"
                            << "  - aedit [field] [values] (modifies current area)\n"
                            << "  - redit [field] [values] (modifies current room)\n"
                            << "  - oedit/nedit [id] [field] [values] (modifies object/npc with specified id in current area)\n"
                            << "  - oreset [id] (creates object reset for current room)\n"
                            << "  - nreset [id] [amount] (creates NPC reset for current room)\n"
                            << "  - alist (lists world areas)\n"
                            << "  - rlist/olist/nlist [areaId] (lists rooms/objects/npcs of area)\n"
                            << "  - goto [id] (moves you to room with id)\n"
                            << "  - reset (triggers world reset)\n";
                break;
            }

            case Command::Acreate: {
                if (this->worldHandler.createArea(param)) {
                    tempMessage << "Area successfully created.\n";
                } else {
                    tempMessage << "Failed to create area.\n";
                }
                break;
            }

            case Command::Rcreate: {
                if (this->worldHandler.createRoom(param)) {
                    tempMessage << "Room successfully created.\n";
                } else {
                    tempMessage << "Failed to create room.\n";
                }
                break;
            }

            case Command::Ocreate: {
                if (this->worldHandler.createObject(param)) {
                    tempMessage << "Object successfully created.\n";
                } else {
                    tempMessage << "Failed to create object.\n";
                }
                break;
            }

            case Command::Ncreate: {
                if (this->worldHandler.createNpc(param)) {
                    tempMessage << "NPC successfully created.\n";
                } else {
                    tempMessage << "Failed to create NPC.\n";
                }
                break;
            }

            case Command::Aedit: {
                auto roomId = this->accountHandler.getRoomIdByClient(client);
                if (this->worldHandler.editArea(roomId, param)) {
                    tempMessage << "Area successfully edited.\n";
                } else {
                    tempMessage << "Failed to edit Area.\n";
                }
                break;
            }

            case Command::Redit: {
                auto roomId = this->accountHandler.getRoomIdByClient(client);
                if (this->worldHandler.editRoom(roomId, param)) {
                    tempMessage << "Room successfully edited.\n";
                } else {
                    tempMessage << "Failed to edit Room.\n";
                }
                break;
            }

            case Command::Oedit: {
                auto roomId = this->accountHandler.getRoomIdByClient(client);
                if (this->worldHandler.editObject(roomId, param)) {
                    tempMessage << "Object successfully edited.\n";
                } else {
                    tempMessage << "Failed to edit Object.\n";
                }
                break;
            }

            case Command::Nedit: {
                auto roomId = this->accountHandler.getRoomIdByClient(client);
                if (this->worldHandler.editNpc(roomId, param)) {
                    tempMessage << "NPC successfully edited.\n";
                } else {
                    tempMessage << "Failed to edit NPC.\n";
                }
                break;
            }

            case Command::Oreset: {
                auto roomId = this->accountHandler.getRoomIdByClient(client);
                if (this->worldHandler.createObjectReset(roomId, param)) {
                    tempMessage << "Reset successfully created.\n";
                } else {
                    tempMessage << "Failed to create reset.\n";
                }
                break;
            }

            case Command::Nreset: {
                auto roomId = this->accountHandler.getRoomIdByClient(client);
                if (this->worldHandler.createNpcReset(roomId, param)) {
                    tempMessage << "Reset successfully created.\n";
                } else {
                    tempMessage << "Failed to create reset.\n";
                }
                break;
            }

            case Command::Alist: {
                tempMessage << "\nArea List\n";
                tempMessage << "---------\n";
                auto areas = this->worldHandler.getWorld().getAreas();
                int count = 1;
                for (const auto &area : areas) {
                    tempMessage << "- " << count << ". " << area.getName() << std::endl;
                    count++;
                }
                break;
            }

            case Command::Rlist: {
                if (!param.empty() && std::all_of(param.begin(), param.end(), ::isdigit)) {
                    unsigned int index = std::stoi(param) - 1;
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
                break;
            }

            case Command::Olist: {
                if (!param.empty() && std::all_of(param.begin(), param.end(), ::isdigit)) {
                    unsigned int index = std::stoi(param) - 1;
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
                break;
            }

            case Command::Nlist: {
                if (!param.empty() && std::all_of(param.begin(), param.end(), ::isdigit)) {
                    unsigned int index = std::stoi(param) - 1;
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
                break;
            }

            case Command::Reset: {
                this->worldHandler.reset();
                tempMessage << "World reset.\n";
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
            || command == Command::Remove
            || command == Command::Goto);

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
