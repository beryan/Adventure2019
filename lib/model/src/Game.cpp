//
// Created by louis on 19/01/19.
//

#include "Game.h"

#include <map>
#include <sstream>
#include <iostream>

using model::Game;
using model::PlayerHandler;
using model::WorldHandler;

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

namespace model {
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

        this->playerHandler = std::make_unique<PlayerHandler>();
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
            if (this->playerHandler->isLoggingIn(disconnectedClient)) {
                this->playerHandler->exitLogin(disconnectedClient);
                std::cout << disconnectedClient.id << " has been removed from login due to disconnect\n";
            }

            if (this->playerHandler->isRegistering(disconnectedClient)) {
                this->playerHandler->exitRegistration(disconnectedClient);
                std::cout << disconnectedClient.id << " has been removed from registration due to disconnect\n";
            }

            if (this->playerHandler->isLoggedIn(disconnectedClient)) {
                this->removeClientFromGame(disconnectedClient);
                this->playerHandler->logoutPlayer(disconnectedClient);
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

            if (this->playerHandler->isLoggingIn(client)) {
                messages.push_back({
                    client,
                    this->playerHandler->processLogin(client, incomingInput.substr(0, incomingInput.find(' ')))
                });

                if (this->playerHandler->isLoggedIn(client)) {
                    this->addClientToGame(client);
                    auto roomID = this->playerHandler->getRoomIdByClient(client);
                    tempMessage << "\n" << this->worldHandler->findRoom(roomID).descToString();
                    messages.push_back({client, tempMessage.str()});
                }

                continue;

            } else if (this->playerHandler->isRegistering(client)) {
                messages.push_back({
                    client,
                    this->playerHandler->processRegistration(client, incomingInput.substr(0, incomingInput.find(' ')))
                });

                if (this->playerHandler->isLoggedIn(client)) {
                    this->addClientToGame(client);
                    auto roomID = this->playerHandler->getRoomIdByClient(client);
                    tempMessage << "\n" << this->worldHandler->findRoom(roomID).descToString();
                    messages.push_back({client, tempMessage.str()});
                }

                continue;
            }

            std::string commandString = lowercase(incomingInput.substr(0, incomingInput.find(' ')));
            std::string username = this->playerHandler->getUsernameByClient(client);
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

            if (!this->playerHandler->isLoggedIn(client)) {
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
                tempMessage << this->playerHandler->processRegistration(client);
                break;

            case Command::Login:
                tempMessage << this->playerHandler->processLogin(client);
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
                tempMessage << this->playerHandler->logoutPlayer(client);
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
                            << "  - " << this->commandHandler.getStringForCommand(Command::Look) << " (displays current location description)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Info) << " (displays current location information)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Exits) << " (displays exits from current location)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Move) << " [direction] (moves you in the direction specified)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Logout) << " (logs you out of the game)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Quit) << " (disconnects you from the game server)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Shutdown) << " (shuts down the game server)\n";
                break;

            case Command::Say: {
                auto roomId = this->playerHandler->getRoomIdByClient(client);
                auto playerIds = this->worldHandler->getNearbyPlayerIds(roomId);

                for (auto playerId : playerIds) {
                    auto connection = this->playerHandler->getClientByPlayerId(playerId);
                    std::ostringstream sayMessage;
                    sayMessage << this->playerHandler->getUsernameByClient(client) << "> " << param << "\n";
                    messages.push_back({connection, sayMessage.str()});
                }

                return messages;
            }

            case Command::Tell: {
                auto username = param.substr(0, param.find(' '));
                auto message = trimWhitespace(param.substr(param.find(' ') + 1));

                for (auto connection: *this->clients) {
                    auto receiver = this->playerHandler->getUsernameByClient(connection);

                    if (receiver == username)  {
                        auto sender = this->playerHandler->getUsernameByClient(client);

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
                    yellMessage << this->playerHandler->getUsernameByClient(client) << "> " << param << "\n";
                    messages.push_back({connection, yellMessage.str()});
                }

                return messages;
            }

            case Command::Look: {
                auto roomID = this->playerHandler->getRoomIdByClient(client);
                tempMessage << "\n" << this->worldHandler->findRoom(roomID).descToString();
                break;
            }

            case Command::Info: {
                auto roomID = this->playerHandler->getRoomIdByClient(client);
                tempMessage << this->worldHandler->findRoom(roomID);
                break;
            }

            case Command::Exits: {
                auto roomID = this->playerHandler->getRoomIdByClient(client);
                tempMessage << "\n" << this->worldHandler->findRoom(roomID).doorsToString();
                break;
            }

            case Command::Move: {
                auto roomID = this->playerHandler->getRoomIdByClient(client);

                if (this->worldHandler->isValidDirection(roomID, param)) {
                    auto playerID = this->playerHandler->getPlayerIdByClient(client);
                    auto destinationID = this->worldHandler->getDestination(roomID, param);
                    this->worldHandler->movePlayer(playerID, roomID, destinationID);
                    this->playerHandler->setRoomIdByClient(client, destinationID);
                    tempMessage << "\n" << this->worldHandler->findRoom(destinationID).descToString();

                } else {
                    tempMessage << "You can't move that way!\n";
                }

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
        this->playerHandler->notifyBootedClients(messages);
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
        auto playerID = this->playerHandler->getPlayerIdByClient(client);
        auto roomID = this->playerHandler->getRoomIdByClient(client);

        this->worldHandler->addPlayer(playerID, roomID);
    }


    void
    Game::removeClientFromGame(Connection client) {
        auto playerID = this->playerHandler->getPlayerIdByClient(client);
        auto roomID = this->playerHandler->getRoomIdByClient(client);
        this->worldHandler->removePlayer(playerID, roomID);
    }


    bool
    Game::isInvalidFormat(const Command &command, const std::string &parameters) {
        bool wrongTellFormat = (command == Command::Tell && parameters.find(' ') == std::string::npos);
        bool isCommandWithParam = (command == Command::Move || command == Command::Say  || command == Command::Yell);

        return (wrongTellFormat || (isCommandWithParam && parameters.empty()));
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
