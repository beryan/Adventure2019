//
// Created by louis on 19/01/19.
//

#include "Game.h"

#include <map>
#include <sstream>
#include <iostream>

using game::Game;
using model::AccountHandler;
using model::WorldHandler;
using action::PlayerAction;

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
    Game::Game(std::vector<Connection> &clients,
               std::vector<Connection> &newClients,
               std::vector<Connection> &disconnectedClients,
               std::function<void(Connection)> &disconnect,
               std::function<void()> &shutdown) {
        this->clients = &clients;
        this->newClients = &newClients;
        this->disconnectedClients = &disconnectedClients;

        this->disconnect = disconnect;
        this->shutdown = shutdown;

        this->accountHandler = std::make_unique<AccountHandler>();
        this->playerHandler = std::make_unique<PlayerAction>();
        this->worldHandler = std::make_unique<WorldHandler>();
    }


    void
    Game::handleConnects(std::deque<Message> &messages) {
        for (auto &newClient : *this->newClients) {
            std::ostringstream introduction;

            introduction << "Welcome to Adventure 2019!\n"
                         << "\n"
                         << "Enter " << "\"" << this->commandHandler.getStringForCommand(Command::Login) << "\" to login to an existing account\n"
                         << "Enter " << "\"" << this->commandHandler.getStringForCommand(Command::Register) << "\" to create a new account\n";

            messages.push_back({newClient, introduction.str()});
        }

        this->newClients->clear();
    }


    void
    Game::handleDisconnects(std::deque<Message> &messages) {

        for (auto &disconnectedClient : *this->disconnectedClients) {
            if (this->accountHandler->isLoggingIn(disconnectedClient)) {
                this->accountHandler->exitLogin(disconnectedClient);
                std::cout << disconnectedClient.id << " has been removed from login due to disconnect\n";
            }

            if (this->accountHandler->isRegistering(disconnectedClient)) {
                this->accountHandler->exitRegistration(disconnectedClient);
                std::cout << disconnectedClient.id << " has been removed from registration due to disconnect\n";
            }

            if (this->accountHandler->isLoggedIn(disconnectedClient)) {
                this->removeClientFromGame(disconnectedClient);
                this->accountHandler->logoutClient(disconnectedClient);
                std::cout << disconnectedClient.id << " has been logged out of the game due to disconnect\n";
            }
        }

        this->disconnectedClients->clear();
    }


    void
    Game::handleIncoming(const std::deque<Message> &incoming, std::deque<Message> &messages) {
        for (const auto& input : incoming) {
            auto client = input.connection;
            auto incomingInput = trimWhitespace(input.text);
            std::ostringstream tempMessage;

            if (this->accountHandler->isLoggingIn(client)) {
                messages.push_back({
                    client,
                    this->accountHandler->processLogin(client, incomingInput.substr(0, incomingInput.find(' ')))
                });

                if (this->accountHandler->isLoggedIn(client)) {
                    this->addClientToGame(client);
                    auto roomId = this->accountHandler->getRoomIdByClient(client);
                    tempMessage << "\n" << this->worldHandler->findRoom(roomId).descToString();
                    messages.push_back({client, tempMessage.str()});
                }

                continue;

            } else if (this->accountHandler->isRegistering(client)) {
                messages.push_back({
                    client,
                    this->accountHandler->processRegistration(client, incomingInput.substr(0, incomingInput.find(' ')))
                });

                if (this->accountHandler->isLoggedIn(client)) {
                    this->addClientToGame(client);
                    auto roomId = this->accountHandler->getRoomIdByClient(client);
                    tempMessage << "\n" << this->worldHandler->findRoom(roomId).descToString();
                    messages.push_back({client, tempMessage.str()});
                }

                continue;
            }

            std::string commandString = lowercase(incomingInput.substr(0, incomingInput.find(' ')));
            std::string username = this->accountHandler->getUsernameByClient(client);
            Command command = this->commandHandler.getCommandForUser(commandString, username);

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

            if (!this->accountHandler->isLoggedIn(client)) {
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
                tempMessage << this->accountHandler->processRegistration(client);
                break;

            case Command::Login:
                tempMessage << this->accountHandler->processLogin(client);
                break;

            case Command::Help:
                tempMessage << "\n"
                            << "********\n"
                            << "* HELP *\n"
                            << "********\n"
                            << "\n"
                            << "COMMANDS:\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Help) << " (shows this help interface)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Register) << " (create a new account)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Login) << " (login to an existing account)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Quit) << " (disconnects you from the game server)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Shutdown) << " (shuts down the game server)\n";
                break;

            default:
                tempMessage << "\n"
                            << "Enter " << "\"" << this->commandHandler.getStringForCommand(Command::Login) << "\" to login to an existing account\n"
                            << "Enter " << "\"" << this->commandHandler.getStringForCommand(Command::Register) << "\" to create a new account\n"
                            << "Enter " << "\"" << this->commandHandler.getStringForCommand(Command::Help) << "\" for a full list of commands\n";
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

        switch (command) {
            case Command::Logout: {
                this->removeClientFromGame(client);
                tempMessage << this->accountHandler->logoutClient(client);
                break;
            }

            case Command::Help:
                tempMessage << "\n"
                            << "********\n"
                            << "* HELP *\n"
                            << "********\n"
                            << "\n"
                            << "COMMANDS:\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Help) << " (shows this help interface)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Say) << " [message] (sends [message] to nearby players in the game)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Tell) << " [username] [message] (sends [message] to [username] in the game)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Yell) << " [message] (sends [message] to other players in the game)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Look) << " (displays current location information)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Exits) << " (displays exits from current location)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Move) << " [direction] (moves you in the direction specified)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Examine) << " [keyword] (examines something or someone)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Talk) << " [keyword] (interacts with NPC)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Take) << " [keyword] (places item in your inventory)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Drop) << " [keyword] (drops item from inventory/equipment)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Wear) << " [keyword] (equips item from your inventory)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Remove) << " [keyword] (unequips item to your inventory)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Inventory) << " (displays your inventory)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Equipment) << " (displays your equipment)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Logout) << " (logs you out of the game)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Quit) << " (disconnects you from the game server)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Shutdown) << " (shuts down the game server)\n";
                break;

            case Command::Say: {
                auto roomId = this->accountHandler->getRoomIdByClient(client);
                auto playerIds = this->worldHandler->getNearbyPlayerIds(roomId);

                for (auto playerId : playerIds) {
                    auto connection = this->accountHandler->getClientByPlayerId(playerId);
                    std::ostringstream sayMessage;
                    sayMessage << this->accountHandler->getUsernameByClient(client) << "> " << param << "\n";
                    messages.push_back({connection, sayMessage.str()});
                }

                return messages;
            }

            case Command::Tell: {
                auto username = param.substr(0, param.find(' '));
                auto message = trimWhitespace(param.substr(param.find(' ') + 1));

                for (auto connection: *this->clients) {
                    auto receiver = this->accountHandler->getUsernameByClient(connection);

                    if (receiver == username)  {
                        auto sender = this->accountHandler->getUsernameByClient(client);

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
                for (auto connection : *this->clients) {
                    std::ostringstream yellMessage;
                    yellMessage << this->accountHandler->getUsernameByClient(client) << "> " << param << "\n";
                    messages.push_back({connection, yellMessage.str()});
                }

                return messages;
            }

            case Command::Look: {
                if (param.empty()) {
                    auto roomId = this->accountHandler->getRoomIdByClient(client);
                    tempMessage << this->worldHandler->findRoom(roomId);
                    break;
                }
            }

            case Command::Examine: {
                auto room = this->worldHandler->findRoom(this->accountHandler->getRoomIdByClient(client));
                auto npcs = room.getNpcs();
                auto objects = room.getObjects();

                if (containsKeyword(npcs, param)) {
                    NPC npc = getItemByKeyword(npcs, param);
                    for (const auto &str : npc.getDescription()) {
                        tempMessage << str << std::endl;
                    }
                } else if (containsKeyword(objects, param)) {
                    Object obj = getItemByKeyword(objects, param);
                    for (const auto &str : obj.getLongDescription()) {
                        tempMessage << str << std::endl;
                    }
                } else {
                    tempMessage << "Invalid keyword.\n";
                }

                break;
            }

            case Command::Exits: {
                auto roomId = this->accountHandler->getRoomIdByClient(client);
                tempMessage << "\n" << this->worldHandler->findRoom(roomId).doorsToString();
                break;
            }

            case Command::Move: {
                auto roomId = this->accountHandler->getRoomIdByClient(client);

                if (this->worldHandler->isValidDirection(roomId, param)) {
                    auto playerId = this->accountHandler->getPlayerIdByClient(client);
                    auto destinationId = this->worldHandler->getDestination(roomId, param);
                    this->worldHandler->movePlayer(playerId, roomId, destinationId);
                    this->accountHandler->setRoomIdByClient(client, destinationId);
                    tempMessage << "\n" << this->worldHandler->findRoom(destinationId).descToString();
                } else {
                    tempMessage << "You can't move that way!\n";
                }

                break;
            }

            case Command::Talk: {
                auto room = this->worldHandler->findRoom(this->accountHandler->getRoomIdByClient(client));
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
                auto roomId = this->accountHandler->getRoomIdByClient(client);
                Room& room = this->worldHandler->findRoom(roomId);
                auto objects = room.getObjects();

                if (containsKeyword(objects, param)) {
                    auto item = getItemByKeyword(objects, param);
                    auto player = this->accountHandler->getPlayerByClient(client);
                    this->worldHandler->removeItem(roomId, item.getId());
                    this->playerHandler->pickupItem(*player, item);
                    tempMessage << "Item taken successfully.\n";
                } else {
                    tempMessage << "Invalid keyword.\n";
                }

                break;
            }

            case Command::Drop: {
                auto player = this->accountHandler->getPlayerByClient(client);
                auto objects = player->getInventory().getVectorInventory();
                auto equip = player->getEquipment().getVectorEquipment();
                objects.insert(objects.end(), equip.begin(), equip.end());

                if (containsKeyword(objects, param)) {
                    auto item = getItemByKeyword(objects, param);
                    auto roomId = this->accountHandler->getRoomIdByClient(client);

                    this->playerHandler->dropItem(*player, item);
                    this->worldHandler->addItem(roomId, item);
                    tempMessage << "Item dropped successfully.\n";
                } else {
                    tempMessage << "Invalid keyword.\n";
                }

                break;
            }

            case Command::Wear: {
                auto player = this->accountHandler->getPlayerByClient(client);
                auto objects = player->getInventory().getVectorInventory();

                if (containsKeyword(objects, param)) {
                    if (this->playerHandler->equipItem(*player, getItemByKeyword(objects, param))) {
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
                auto player = this->accountHandler->getPlayerByClient(client);
                auto objects = player->getEquipment().getVectorEquipment();

                if (containsKeyword(objects, param)) {
                    this->playerHandler->unequipItem(*player, getItemByKeyword(objects, param));
                    tempMessage << "Item unequipped successfully.\n";
                } else {
                    tempMessage << "Invalid keyword.\n";
                }

                break;
            }

            case Command::Inventory: {
                tempMessage << this->accountHandler->getPlayerByClient(client)->getInventory();
                break;
            }

            case Command::Equipment: {
                tempMessage << this->accountHandler->getPlayerByClient(client)->getEquipment();
                break;
            }

            case Command::Debug: {
                tempMessage << this->worldHandler->getWorld();
                break;
            }

            default:
                tempMessage << "\nEnter " << "\"" << this->commandHandler.getStringForCommand(Command::Help) << "\" for a full list of commands\n";
                break;
        }

        messages.push_back({client, tempMessage.str()});

        return messages;
    }


    void
    Game::handleOutgoing(std::deque<Message> &messages) {
        this->accountHandler->notifyBootedClients(messages);
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
        auto playerId = this->accountHandler->getPlayerIdByClient(client);
        auto roomId = this->accountHandler->getRoomIdByClient(client);
        this->worldHandler->addPlayer(roomId, playerId);
    }


    void
    Game::removeClientFromGame(Connection client) {
        auto playerId = this->accountHandler->getPlayerIdByClient(client);
        auto roomId = this->accountHandler->getRoomIdByClient(client);
        this->worldHandler->removePlayer(roomId, playerId);
    }


    bool
    Game::isInvalidFormat(const Command &command, const std::string &parameters) {
        bool wrongTellFormat = (command == Command::Tell && parameters.find(' ') == std::string::npos);
        bool isCommandWithParam = (command == Command::Say
            || command == Command::Yell
            || command == Command::Move
            || command == Command::Examine
            || command == Command::Talk
            || command == Command::Take
            || command == Command::Drop
            || command == Command::Wear
            || command == Command::Remove);

        return (wrongTellFormat || (isCommandWithParam && parameters.empty()));
    }


    template <typename T>
    bool
    Game::containsKeyword(const std::vector<T> &objects, const std::string &keyword) {
        auto it = std::find_if(objects.begin(), objects.end(), [&keyword](const T &obj) {return obj.containsKeyword(keyword);});
        return (it != objects.end());
    }


    template <typename T>
    T
    Game::getItemByKeyword(const std::vector<T> &objects, const std::string &keyword) {
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
