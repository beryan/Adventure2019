//
// Created by louis on 19/01/19.
//

#include "Game.h"

#include <iostream>
#include <boost/algorithm/string.hpp>
#include <vector>

using game::Game;

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
    Game::Game(ConnectionHandler &connectionHandler) :
            connectionHandler(connectionHandler),
            magicHandler(this->accountHandler),
            combatHandler(this->accountHandler, this->worldHandler),
            commandExecutor(connectionHandler, accountHandler, magicHandler, combatHandler, worldHandler,
                            aliasManager,
                            commandParser),
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
                this->combatHandler.handleLogout(disconnectedClient);
                this->magicHandler.handleLogout(disconnectedClient);
                this->removeClientFromGame(disconnectedClient);
                std::cout << disconnectedClient.id << " has been logged out of the game due to disconnect\n";
            }
        }

        this->connectionHandler.getDisconnectedClients().clear();
    }

    void
    Game::handleIncoming(const std::deque<Message> &incoming, std::deque<Message> &messages) {
        for (const auto &input : incoming) {
            auto client = input.connection;
            auto incomingInput = trimWhitespace(input.text);
            std::ostringstream tempMessage;

            if (this->accountHandler.isLoggingIn(client)) {
                auto username = incomingInput.substr(0, incomingInput.find(' '));
                messages.push_back({client, this->accountHandler.processLogin(client, username)});

                if (this->accountHandler.isLoggedIn(client)) {
                    this->addClientToGame(client);
                    auto roomId = this->accountHandler.getRoomIdByClient(client);
                    tempMessage << "\n" << this->worldHandler.findRoom(roomId).descToString();
                    messages.push_back({client, tempMessage.str()});
                }

                continue;

            } else if (this->accountHandler.isRegistering(client)) {
                auto username = incomingInput.substr(0, incomingInput.find(' '));
                std::string result = this->accountHandler.processRegistration(client, username);
                messages.push_back({client, result});
                if (this->accountHandler.isLoggedIn(client)) {
                    this->addClientToGame(client);
                    auto roomId = this->accountHandler.getRoomIdByClient(client);
                    tempMessage << "\n" << this->worldHandler.findRoom(roomId).descToString();
                    messages.push_back({client, tempMessage.str()});
                }

                continue;
            }

            std::string commandString = boost::algorithm::to_lower_copy(
                    incomingInput.substr(0, incomingInput.find(' ')));
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
                    this->connectionHandler.disconnectClient(input.connection);
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
                            << "  - " << this->commandParser.getStringForCommand(Command::Help)
                            << " (shows this help interface)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Register)
                            << " (create a new account)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Login)
                            << " (login to an existing account)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Quit)
                            << " (disconnects you from the game server)\n"
                            << "  - " << this->commandParser.getStringForCommand(Command::Shutdown)
                            << " (shuts down the game server)\n";
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

        return this->commandExecutor.executeCommand(client, command, params);
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

        for (auto const&[client, message] : clientMessages) {
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

    std::string
    Game::removeClientFromGame(Connection client) {
        magicHandler.handleLogout(client);
        auto playerId = this->accountHandler.getPlayerIdByClient(client);
        auto roomId = this->accountHandler.getRoomIdByClient(client);
        this->worldHandler.removePlayer(roomId, playerId);
        return accountHandler.logoutClient(client);
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
                                   || command == Command::Chat);

        return (wrongFormat || (isCommandWithParam && parameters.empty()));
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
