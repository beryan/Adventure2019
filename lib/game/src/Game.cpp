//
// Created by louis on 19/01/19.
//

#include "Game.h"
#include "DataManager.h"

#include <iostream>
#include <boost/algorithm/string.hpp>
#include <vector>

using game::Game;

namespace game {
    Game::Game(ConnectionHandler &connectionHandler) :
            connectionHandler(connectionHandler),
            avatarHandler(this->accountHandler),
            magicHandler(this->accountHandler,this->combatHandler),
            combatHandler(this->accountHandler, this->worldHandler),
            commandExecutor(connectionHandler, accountHandler, avatarHandler, magicHandler, combatHandler,
                            worldHandler, aliasManager, commandParser),
            running(true) {};

    void
    Game::handleConnects(std::deque<Message> &messages) {
        for (auto &newClient : connectionHandler.getNewClients()) {
            std::ostringstream introduction;

            introduction << "Welcome to Adventure 2019!\n"
                         << "\n"
                         << "Enter " << "\"" << this->commandParser.getStringForCommand(Command::Login)
                         << "\" to login to an existing account\n"
                         << "Enter " << "\"" << this->commandParser.getStringForCommand(Command::Register)
                         << "\" to create a new account\n";

            messages.push_back({newClient, introduction.str()});
        }

        this->connectionHandler.getNewClients().clear();
    }

    void
    Game::handleDisconnects(std::deque<Message> &messages) {
        for (auto &disconnectedClient : this->connectionHandler.getDisconnectedClients()) {
            if (this->accountHandler.isLoggingIn(disconnectedClient)) {
                this->accountHandler.exitLogin(disconnectedClient);
                std::cout << disconnectedClient.id << " has been removed from login due to disconnect\n";
            }

            if (this->accountHandler.isRegistering(disconnectedClient)) {
                this->accountHandler.exitRegistration(disconnectedClient);
                std::cout << disconnectedClient.id << " has been removed from registration due to disconnect\n";
            }

            if (this->accountHandler.isLoggedIn(disconnectedClient)) {
                this->removeClientFromGame(disconnectedClient);
                std::cout << disconnectedClient.id << " has been logged out of the game due to disconnect\n";
            }

            if (this->avatarHandler.isCreatingAvatar(disconnectedClient)) {
                this->avatarHandler.exitCreation(disconnectedClient);
                std::cout << disconnectedClient.id << " has been removed from avatar creation due to disconnect\n";
            }
        }

        this->connectionHandler.getDisconnectedClients().clear();
    }

    void
    Game::handleIncoming(const std::deque<Message> &incoming, std::deque<Message> &messages) {
        for (const auto &input : incoming) {
            auto client = input.connection;
            auto incomingInput = boost::algorithm::trim_copy(input.text);
            std::ostringstream tempMessage;

            if (this->accountHandler.isLoggingIn(client)) {
                auto username = incomingInput.substr(0, incomingInput.find(' '));
                messages.push_back({client, this->accountHandler.processLogin(client, username)});

                if (this->accountHandler.isLoggedIn(client)) {
                    bool hasCreatedAvatar = this->accountHandler.getPlayerByClient(client).getAvatar().isDefined();
                    if (!hasCreatedAvatar) {
                        messages.push_back({
                            client,
                            this->avatarHandler.processCreation(client)
                        });

                        continue;
                    }

                    this->addClientToGame(client);
                    auto roomID = this->accountHandler.getRoomIdByClient(client);
                    tempMessage << this->worldHandler.findRoom(roomID).descToString();
                    messages.push_back({client, tempMessage.str()});
                }

                continue;

            } else if (this->accountHandler.isRegistering(client)) {
                auto username = incomingInput.substr(0, incomingInput.find(' '));
                std::string result = this->accountHandler.processRegistration(client, username);
                messages.push_back({client, result});
                if (this->accountHandler.isLoggedIn(client)) {
                    messages.push_back({
                        client,
                        this->avatarHandler.processCreation(client)
                    });
                }

                continue;

            } else if (this->avatarHandler.isCreatingAvatar(client)) {
                messages.push_back({
                    client,
                    this->avatarHandler.processCreation(client, incomingInput.substr(0, incomingInput.find(' ')))
                });

                // Add player to game after finishing avatar creation
                if (!this->avatarHandler.isCreatingAvatar(client)) {
                    this->addClientToGame(client);
                    auto roomID = this->accountHandler.getRoomIdByClient(client);
                    tempMessage << this->worldHandler.findRoom(roomID).descToString();
                    messages.push_back({client, tempMessage.str()});
                }

                continue;
            }

            std::string commandString = boost::algorithm::to_lower_copy(
                    incomingInput.substr(0, incomingInput.find(' ')));
            std::string username = this->accountHandler.getUsernameByClient(client);
            Command command = this->aliasManager.getCommandForUser(commandString, username);
            std::string parameters;

            std::vector<std::string> dir = {"north", "south", "east", "west", "northeast", "southeast",
                                            "northwest", "southwest", "up", "down"};
            if (std::find(dir.begin(), dir.end(), commandString) != dir.end()) {
                command = Command::Move;
                parameters = commandString;

            } else if (command == Command::InvalidCommand) {
                tempMessage << "The word \"" << commandString << "\" is not a valid command.\n";
                messages.push_back({client, tempMessage.str()});
                continue;

            } else if (incomingInput.find(' ') != std::string::npos) {
                parameters = boost::algorithm::trim_copy(incomingInput.substr(incomingInput.find(' ') + 1));
            }

            if (command == Command::Quit) {
                this->connectionHandler.disconnectClient(input.connection);
                continue;
            }

            if (!this->accountHandler.isLoggedIn(client) && !this->avatarHandler.isCreatingAvatar(client)) {
                messages.push_back(this->executeMenuAction(client, command, parameters));
            } else {
                if (game::isInvalidRole(command, this->accountHandler.getPlayerByClient(client).getRole())) {
                    tempMessage << "You don't have access to \""
                                << this->commandParser.getStringForCommand(command) << "\".\n";
                    messages.push_back({client, tempMessage.str()});
                } else if (game::isInvalidFormat(command, parameters)) {
                    tempMessage << "Invalid format for command \""
                                << this->commandParser.getStringForCommand(command) << "\".\n";
                    messages.push_back({client, tempMessage.str()});
                } else if (game::isRoleCommand(command) && this->magicHandler.isBodySwapped(client)) {
                    tempMessage << "You don't have access to \""
                                << this->commandParser.getStringForCommand(command) << "\" while body swapped.\n";
                    messages.push_back({client, tempMessage.str()});

                } else {
                    if (command == Command::Shutdown) {
                        World world = this->worldHandler.getWorld();
                        DataManager::writeWorldToFile(world, DataManager::JSON);
                        
                        auto players = this->accountHandler.getAllPlayers();
                        DataManager::saveRegisteredUsers(players);

                        std::cout << "Shutting down.\n";
                        this->shutdown();
                        return;
                    }

                    auto responseList = this->executeInGameAction(client, command, parameters);

                    for (const auto &response : responseList) {
                        messages.push_back(response);
                    }
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
                            << "  - " << this->commandParser.getStringForCommand(Command::Help)
                            << " (shows this help interface)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Register)
                            << " (create a new account)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Login)
                            << " (login to an existing account)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Quit)
                            << " (disconnects you from the game server)\n";
                break;

            default:
                tempMessage << "\n"
                            << "Enter " << "\"" << this->commandParser.getStringForCommand(Command::Login)
                            << "\" to login to an existing account\n"
                            << "Enter " << "\"" << this->commandParser.getStringForCommand(Command::Register)
                            << "\" to create a new account\n"
                            << "Enter " << "\"" << this->commandParser.getStringForCommand(Command::Help)
                            << "\" for a full list of commands\n";
                break;
        }

        return {client, tempMessage.str()};
    }

    std::vector<Message>
    Game::executeInGameAction(const Connection &client, const Command &command, const std::string &param) {
        if (command == Command::Logout) {
            std::string msg = this->removeClientFromGame(client);
            return std::vector<Message>{{client, msg}};
        }

        std::vector<std::string> params;
        boost::split(params, param, boost::is_any_of("\t "));

        try {
            return this->commandExecutor.executeCommand(client, command, params);
        } catch (const std::out_of_range &exception) {
            return {{client,"Failed to execute command!"}};
        }
    }

    void
    Game::handleOutgoing(std::deque<Message> &messages) {
        this->accountHandler.notifyBootedClients(messages);
        this->magicHandler.processCycle(messages);
        this->combatHandler.processCycle(messages);
        this->worldHandler.processResets();
    }

    std::deque<Message>
    Game::formMessages(std::deque<Message> &messages) {
        std::deque<Message> outgoing;
        std::map<Connection, std::ostringstream> clientMessages;

        for (const auto &message : messages) {
            clientMessages[message.connection] << message.text;
        }

        for (auto const&[client, message] : clientMessages) {
            outgoing.push_back({client, message.str()});
        }

        return outgoing;
    }

    void
    Game::addClientToGame(Connection client) {
        auto playerId = this->accountHandler.getPlayerIdByClient(client);
        auto roomId = this->accountHandler.getRoomIdByClient(client);

        if (!this->worldHandler.roomExists(roomId)) {
            assert(!this->worldHandler.getWorld().getAreas().empty());
            auto firstArea = this->worldHandler.getWorld().getAreas().at(0);
            assert(!firstArea.getRooms().empty());
            roomId = firstArea.getRooms().at(0).getId();
            this->accountHandler.setRoomIdByClient(client, roomId);
        }

        this->worldHandler.addPlayer(roomId, playerId);
    }

    std::string
    Game::removeClientFromGame(Connection client) {
        this->combatHandler.handleLogout(client);
        this->magicHandler.handleLogout(client);
        auto playerId = this->accountHandler.getPlayerIdByClient(client);
        auto roomId = this->accountHandler.getRoomIdByClient(client);
        this->worldHandler.removePlayer(roomId, playerId);
        return this->accountHandler.logoutClient(client);
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

    bool Game::isRunning() const {
        return running;
    }

    void Game::shutdown() {
        running = false;
    }
}
