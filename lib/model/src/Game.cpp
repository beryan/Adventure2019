//
// Created by louis on 19/01/19.
//

#include "Game.h"
#include "Room.h"

#include <map>
#include <sstream>
#include <iostream>

using model::Game;
using model::Room;
using model::PlayerHandler;
using model::WorldHandler;

std::string
lowercase(std::string string) {
    std::transform(string.begin(), string.end(), string.begin(), ::tolower);
    return string;
}

std::string
trimWhitespace(std::string string) {
    string.erase(string.begin(), std::find_if(string.begin(), string.end(), [](int ch) {
        return !std::isspace(ch);
    }));

    string.erase(std::find_if(string.rbegin(), string.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), string.end());

    return string;
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
                         << "Enter \"login\" to sign into an existing account\n"
                         << "Enter \"register\" to make a new account\n";

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

            if (this->playerHandler->isLoggingIn(client)) {
                messages.push_back({
                    client,
                    this->playerHandler->processLogin(client, incomingInput.substr(0, incomingInput.find(' '))),
                });
                continue;

            } else if (this->playerHandler->isRegistering(client)) {
                messages.push_back({
                    client,
                    this->playerHandler->processRegistration(client, incomingInput.substr(0, incomingInput.find(' '))),
                });
                continue;
            }

            std::string commandString = lowercase(incomingInput.substr(0, incomingInput.find(' ')));

            if (!this->commandMap.count(commandString)) {
                std::ostringstream tempMessage;
                tempMessage << "The word \"" << commandString << "\" is not a valid command.\n";
                messages.push_back({client, tempMessage.str()});
                continue;
            }

            std::string parameters;

            if (input.text.find(' ') != std::string::npos) {
                parameters = trimWhitespace(incomingInput.substr(incomingInput.find(' ') + 1));
            }

            Command command = this->commandMap.at(commandString);

            if (command == Command::TELL && parameters.find(' ') == std::string::npos) {
                std::ostringstream tempMessage;
                tempMessage << "Invalid format for command \"" << commandString << "\".\n";
                messages.push_back({client, tempMessage.str()});
                continue;
            }

            switch (command) {
                case Command::QUIT: {
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
                            << "  - " << this->getCommandWords(Command::SHUTDOWN) << " (shuts down the game server)\n"
                            << "\n";
                break;

            default:
                tempMessage << "Enter " << "\"" << this->getCommandWords(Command::LOGIN) << "\" to log into an existing account\n"
                            << "Enter " << "\"" << this->getCommandWords(Command::REGISTER) << "\" to create a new account\n"
                            << "Enter " << "\"" << this->getCommandWords(Command::HELP) << "\" for a full list of commands.\n";
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
            case Command::LOGOUT:
                tempMessage << this->playerHandler->logoutPlayer(client);
                break;

            case Command::HELP:
                tempMessage << "\n"
                            << "********\n"
                            << "* HELP *\n"
                            << "********\n"
                            << "\n"
                            << "COMMANDS:\n"
                            << "  - " << this->getCommandWords(Command::HELP) << " (shows this help interface)\n"
                            << "  - " << this->getCommandWords(Command::SAY) << " [message] (sends [message] to other players in the game)\n"
                            << "  - " << this->getCommandWords(Command::TELL) << " [username] [message] (sends [message] to [username] in the game)\n"
                            << "  - " << this->getCommandWords(Command::LOOK) << " (displays current location information)\n"
                            << "  - " << this->getCommandWords(Command::MOVE) << " [direction] (moves you in the direction specified)\n"
                            << "  - " << this->getCommandWords(Command::LOGOUT) << " (logs you out of the server)\n"
                            << "  - " << this->getCommandWords(Command::QUIT) << " (disconnects you from the game server)\n"
                            << "  - " << this->getCommandWords(Command::SHUTDOWN) << " (shuts down the game server)\n"
                            << "\n";
                break;

            case Command::SAY: {
                if (param.length() == 0) {
                    tempMessage << "That's great but what would you like to say?\n";
                    break;
                }

                for (const auto gameClient : *this->clients) {
                    if (this->playerHandler->isLoggedIn(gameClient)) {
                        tempMessage << this->playerHandler->getUsernameByClient(client) << "> " << param << "\n";
                        messages.push_back({gameClient, tempMessage.str()});
                    }
                }

                return messages;
            }

            case Command::TELL: {
                auto username = param.substr(0, param.find(' '));
                auto message = param.substr(param.find(' ') + 1);

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
                break;
        }

        messages.push_back({client, tempMessage.str()});
        
        return messages;
    }

    
    void
    Game::handleOutgoing(std::deque<Message> &messages) {

        this->playerHandler->notifyBootedClients(messages);

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


    std::deque<Message>
    Game::processCycle(std::deque<Message> &incoming) {
        std::deque<Message> responses;

        this->handleConnects(responses);
        this->handleDisconnects(responses);
        this->handleIncoming(incoming, responses);
        this->handleOutgoing(responses);

        return responses;
    }
}
