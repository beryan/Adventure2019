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
trimWhitespace(std::string string) {
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
    Game::handleConnects(std::deque<Message> &Messages) {
        for (auto &newClient : *this->newClients) {
            std::ostringstream introduction;

            introduction << "Welcome to Adventure 2019!\n"
                         << "\n"
                         << "Enter \"login\" to login to an existing account\n"
                         << "Enter \"register\" to create a new account\n";

            Messages.push_back({newClient, introduction.str()});
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
                removeClientFromGame(disconnectedClient);
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
                    this->playerHandler->processLogin(client, incomingInput.substr(0, incomingInput.find(' '))),
                });
                if (this->playerHandler->isLoggedIn(client)) {
                    addClientToGame(client);
                    auto roomID = this->playerHandler->getRoomIdByClient(client);
                    tempMessage << this->worldHandler->findRoom(roomID);
                    messages.push_back({client, tempMessage.str()});
                }
                continue;

            } else if (this->playerHandler->isRegistering(client)) {
                messages.push_back({
                    client,
                    this->playerHandler->processRegistration(client, incomingInput.substr(0, incomingInput.find(' '))),
                });
                if (this->playerHandler->isLoggedIn(client)) {
                    addClientToGame(client);
                    auto roomID = this->playerHandler->getRoomIdByClient(client);
                    tempMessage << this->worldHandler->findRoom(roomID);
                    messages.push_back({client, tempMessage.str()});
                }
                continue;
            }

            std::string commandString = lowercase(incomingInput.substr(0, incomingInput.find(' ')));

            if (!this->commandMap.count(commandString)) {
                tempMessage << "The word \"" << commandString << "\" is not a valid command.\n";
                messages.push_back({client, tempMessage.str()});
                continue;
            }

            std::string parameters = "";

            if (incomingInput.find(' ') != std::string::npos) {
                parameters = trimWhitespace(incomingInput.substr(incomingInput.find(' ') + 1));
            }

            Command command = this->commandMap.at(commandString);

            switch (command) {
                case Command::QUIT: {
                    removeClientFromGame(client);
                    this->disconnect(input.connection);
                    continue;
                }

                case Command::SHUTDOWN: {
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
                if (isInvalidFormat(command, parameters)) {
                    tempMessage << "Invalid format for command \"" << commandString << "\".\n";
                    messages.push_back({client, tempMessage.str()});
                    continue;
                }
                auto responseList = this->executeInGameAction(client, command, parameters);
                for (auto response : responseList) {
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
            case Command::REGISTER:
                tempMessage << this->playerHandler->processRegistration(client);
                break;

            case Command::LOGIN:
                tempMessage << this->playerHandler->processLogin(client);
                break;

            case Command::HELP:
                tempMessage << "\n"
                            << "********\n"
                            << "* HELP *\n"
                            << "********\n"
                            << "\n"
                            << "COMMANDS:\n"
                            << "  - " << this->getCommandWords(Command::HELP) << " (shows this help interface)\n"
                            << "  - " << this->getCommandWords(Command::REGISTER) << " (create a new account)\n"
                            << "  - " << this->getCommandWords(Command::LOGIN) << " (login to an existing account)\n"
                            << "  - " << this->getCommandWords(Command::QUIT) << " (disconnects you from the game server)\n"
                            << "  - " << this->getCommandWords(Command::SHUTDOWN) << " (shuts down the game server)\n";
                break;

            default:
                tempMessage << "\nEnter " << "\"" << this->getCommandWords(Command::LOGIN) << "\" to login to an existing account\n"
                            << "Enter " << "\"" << this->getCommandWords(Command::REGISTER) << "\" to create a new account\n"
                            << "Enter " << "\"" << this->getCommandWords(Command::HELP) << "\" for a full list of commands\n";
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
            case Command::LOGOUT: {
                removeClientFromGame(client);
                tempMessage << this->playerHandler->logoutPlayer(client);
                break;
            }

            case Command::HELP:
                tempMessage << "\n"
                            << "********\n"
                            << "* HELP *\n"
                            << "********\n"
                            << "\n"
                            << "COMMANDS:\n"
                            << "  - " << this->getCommandWords(Command::HELP) << " (shows this help interface)\n"
                            << "  - " << this->getCommandWords(Command::SAY) << " [message] (sends [message] to close by players in the game)\n"
                            << "  - " << this->getCommandWords(Command::TELL) << " [username] [message] (sends [message] to [username] in the game)\n"
														<< "  - " << this->getCommandWords(Command::YELL) << " [message] (sends [message] to other players in the game)\n"
                            << "  - " << this->getCommandWords(Command::LOOK) << " (displays current location information)\n"
                            << "  - " << this->getCommandWords(Command::MOVE) << " [direction] (moves you in the direction specified)\n"
                            << "  - " << this->getCommandWords(Command::LOGOUT) << " (logs you out of the game)\n"
                            << "  - " << this->getCommandWords(Command::QUIT) << " (disconnects you from the game server)\n"
                            << "  - " << this->getCommandWords(Command::SHUTDOWN) << " (shuts down the game server)\n";
                break;

            case Command::SAY: {
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

            case Command::TELL: {
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

            case Command::YELL: {
                for (auto connection : *this->clients) {
                    std::ostringstream yellMessage;
                    yellMessage << this->playerHandler->getUsernameByClient(client) << "> " << param << "\n";
                    messages.push_back({connection, yellMessage.str()});
                }

                return messages;
            }

            case Command::LOOK: {
                auto roomID = this->playerHandler->getRoomIdByClient(client);
                tempMessage << this->worldHandler->findRoom(roomID);
                break;
            }

            case Command::MOVE: {
                auto roomID = this->playerHandler->getRoomIdByClient(client);
                if (this->worldHandler->isValidDirection(roomID, param)) {
                    auto playerID = this->playerHandler->getPlayerIdByClient(client);
                    auto destinationID = this->worldHandler->getDestination(roomID, param);
                    this->worldHandler->movePlayer(playerID, roomID, destinationID);
                    this->playerHandler->setRoomIdByClient(client, destinationID);
                    tempMessage << this->worldHandler->findRoom(destinationID);
                } else {
                    tempMessage << "You can't move that way!\n";
                }
                break;
            }

            default:
                tempMessage << "\nEnter " << "\"" << this->getCommandWords(Command::HELP) << "\" for a full list of commands\n";
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


    std::string
    Game::getCommandWords(Command command) {
        std::vector<std::string> words = this->commandWordsMap.at(command);
        std::ostringstream tempMessage;

        for (unsigned int i = 0; i < words.size(); ++i) {
            tempMessage << words[i];

            if (i < (words.size() - 1)) {
                tempMessage << ", ";
            }
        }

        return tempMessage.str();
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
        bool wrongTellFormat = (command == Command::TELL && parameters.find(' ') == std::string::npos);
        bool isCommandWithParam = (command == Command::MOVE || command == Command::SAY  || command == Command::YELL);
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
