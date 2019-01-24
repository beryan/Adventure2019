//
// Created by louis on 19/01/19.
//

#include "Game.h"
#include "Room.h"
#include "WorldHandler.h"

#include <map>
#include <sstream>
#include <iostream>

using model::Game;
using model::WorldHandler;
using model::Room;

std::string
lowercase(std::string string) {
    std::transform(string.begin(), string.end(), string.begin(), ::tolower);
    return string;
}

namespace model {
    const char* const Game::COMMAND_SHUTDOWN = "shutdown";
    const char* const Game::COMMAND_QUIT     = "quit";
    const char* const Game::COMMAND_SAY      = "say";
    const char* const Game::COMMAND_HELP     = "help";
    const char* const Game::COMMAND_REGISTER = "register";
    const char* const Game::COMMAND_LOGIN    = "login";
    const char* const Game::COMMAND_LOGOUT   = "logout";
    const char* const Game::COMMAND_INFO     = "info";
    const char* const Game::COMMAND_START    = "start";

    Game::Game(std::vector<Connection> &clients,
               std::vector<uintptr_t> &newClientIds,
               std::vector<uintptr_t> &disconnectedClientIds,
               std::function<void(Connection)> &disconnect,
               std::function<void()> &shutdown) {
        this->clients = &clients;
        this->newClientIds = &newClientIds;
        this->disconnectedClientIds = &disconnectedClientIds;
        this->disconnect = disconnect;
        this->shutdown = shutdown;

        /* Login/Register member variables */
        this->nextId = 1;
        this->usernameToPlayer = {};
        this->idToPlayer = {};
        this->activeIdToClient = {};
        this->activeClientToId = {};
        /* End */
    }

    void
    Game::handleConnects(std::deque<Response> &results) {
        while (!this->newClientIds->empty()) {
            auto clientId = this->newClientIds->back();

            std::ostringstream introduction;

            introduction << "Welcome to Adventure 2019!\n"
                         << "\n"
                         << "Enter \"login [username] [password]\" to sign into an existing account\n"
                         << "Enter \"register [username] [password]\" to make a new account\n";

            results.push_back({clientId, introduction.str(), true});

            this->newClientIds->pop_back();
        }
    }

    void
    Game::handleDisconnects(std::deque<Response> &results) {
        while (!this->disconnectedClientIds->empty()) {
            auto clientId = this->disconnectedClientIds->back();

            /* Login/Register code */
            if (this->activeClientToId.count(clientId)) {
                IdType playerId = this->activeClientToId.at(clientId);
                std::string username = this->idToPlayer.at(playerId).getUsername();

                this->activeClientToId.erase(clientId);

                std::cout << username << " has been logged out of the game due to disconnect\n";
            }
            /* End */

            this->disconnectedClientIds->pop_back();
        }
    }

    void
    Game::handleIncoming(const std::deque<Message> &incoming, std::deque<Response> &results) {
        for (auto& input : incoming) {
            uintptr_t clientId = input.connection.id;

            std::ostringstream tempMessage;
            std::string command = lowercase(input.text.substr(0, input.text.find(' ')));
            std::string param;
            bool isLocal = true;

            if (input.text.find(' ') != std::string::npos) {
                param = input.text.substr(input.text.find(' ') + 1);
            }

            if (command == COMMAND_QUIT) {
                this->disconnect(input.connection);
                continue;

            } else if (command == COMMAND_SHUTDOWN) {
                std::cout << "Shutting down.\n";
                this->shutdown();
                return;
            }

            // Main menu commands
            if (!this->activeClientToId.count(clientId)) {
                if (command == COMMAND_REGISTER) {
                    /* Login/Register code */
                    std::string username = param.substr(0, param.find(' '));
                    std::string password;

                    if (param.find(' ') != std::string::npos) {
                        password = param.substr(param.find(' ') + 1);

                    } else {
                        tempMessage << "Missing fields for registration.\n";
                        results.push_back({clientId, tempMessage.str(), true});
                        continue;
                    }

                    if (this->usernameToPlayer.count(username)) {
                        tempMessage << "The username " << username << " has already been taken,"
                                    << " please use a different username.\n";
                        results.push_back(Response{clientId, tempMessage.str(), true});
                        continue;
                    }

                    int currentId = this->nextId;
                    ++this->nextId;

                    this->idToPlayer.emplace(this->nextId, Player(currentId, username, password));
                    Player* newPlayer = &this->idToPlayer.at(currentId);
                    
                    this->usernameToPlayer.emplace(username, newPlayer);
                    this->loginPlayer(clientId, currentId);

                    std::cout << username << " has been registered to the game with player ID " << newPlayer->getId() << "\n";
                    tempMessage << "Account created!\n";
                    /* End */

                } else if (command == COMMAND_LOGIN) {
                    /* Login/Register code */
                    std::string username = param.substr(0, param.find(' '));
                    std::string password;
                    bool success = false;

                    if (param.find(' ') != std::string::npos) {
                        password = param.substr(param.find(' ') + 1);

                        if (this->usernameToPlayer.count(username)) {
                            if (this->usernameToPlayer.at(username)->getPassword() == password) {

                                IdType playerId = this->usernameToPlayer.at(username)->getId();

                                if (!this->activeIdToClient.count(playerId)) {
                                    this->loginPlayer(clientId, playerId);

                                } else {
                                    // Player is already being used by a client, logout associated client
                                    // and login with new client
                                    results.push_back({this->activeIdToClient.at(playerId),
                                                         "You have been logged out due to being logged in elsewhere.\n\n",
                                                         true});

                                    this->logoutPlayer(playerId);
                                    this->loginPlayer(clientId, playerId);

                                    std::cout << username << " is now being used by " << clientId << "\n";
                                }

                                success = true;
                            }
                        }
                    }

                    if (success) {
                        std::cout << username << " has entered the game\n";
                        tempMessage << "Logged in successfully!\n";

                    } else {
                        tempMessage << "Invalid username or password.\n";
                    }
                    /* End */

                } else if (command == COMMAND_HELP) {
                    tempMessage << "\n"
                                << "********\n"
                                << "* HELP *\n"
                                << "********\n"
                                << "\n"
                                << "COMMANDS:\n"
                                << "  - " << COMMAND_HELP     << " (shows this help interface)\n"
                                << "  - " << COMMAND_REGISTER << " (create a new account with [username] [password])\n"
                                << "  - " << COMMAND_LOGIN    << " (login to an account with [username] [password])\n"
                                << "  - " << COMMAND_QUIT     << " (disconnects you from the game server)\n"
                                << "  - " << COMMAND_SHUTDOWN << " (shuts down the game server)\n"
                                << "\n";

                } else {
                    tempMessage << "The word \"" << command << "\" is not a valid command.\n"
                                << "\n"
                                << "Enter " << "\"" << COMMAND_LOGIN << " [username] [password]\" to log into an existing account\n"
                                << "Enter " << "\"" << COMMAND_REGISTER << "[username] [password]\" to create a new account\n"
                                << "Enter " << "\"" << COMMAND_HELP << "\" for a full list of commands.\n";

                }

                results.push_back({clientId, tempMessage.str(), true});
                continue;
            }

            // In-game commands

            /* Login/Register code */
            IdType playerId = this->activeClientToId.at(clientId);
            /* End */

            if (command == COMMAND_START) {
                WorldHandler wh;

            } else if (command == COMMAND_SAY) {
                isLocal = false;
                tempMessage << this->idToPlayer.at(playerId).getUsername() << "> " << param << "\n";

            } else if (command == COMMAND_LOGOUT) {
                /* Login/Register code */
                std::string username = this->idToPlayer.at(playerId).getUsername();

                this->logoutPlayer(playerId);
                this->newClientIds->push_back(clientId);

                std::cout << username << " has logged out of the game\n";
                tempMessage << "Logged out successfully.\n"
                            << "\n";
                /* End */

            } else if (command == COMMAND_HELP) {
                tempMessage << "\n"
                            << "********\n"
                            << "* HELP *\n"
                            << "********\n"
                            << "\n"
                            << "COMMANDS:\n"
                            << "  - " << COMMAND_HELP     << " (shows this help interface)\n"
                            << "  - " << COMMAND_SAY      << " [message] (sends [message] to other players in the game)\n"
                            << "  - " << COMMAND_LOGOUT   << " (logs you out of the server)\n"
                            << "  - " << COMMAND_QUIT     << " (disconnects you from the game server)\n"
                            << "  - " << COMMAND_SHUTDOWN << " (shuts down the game server)\n"
                            << "  - " << COMMAND_INFO     << " (displays current location information)\n"
                            << "\n";

            } else if (command == COMMAND_INFO) {
                model::Room stubRoom = model::Room();
                stubRoom.createStub();
                tempMessage << stubRoom;

            } else {
                tempMessage << "The word \"" << command << "\" is not a valid command. Enter \"help\" for a list of commands.\n";
            }

            results.push_back({clientId, tempMessage.str(), isLocal});
        }
    }

    void
    Game::handleOutgoing(std::deque<model::Response> &results) {
        /*
         * Call handler methods that push Responses without requiring user input here
         */
    }

    std::deque<Message>
    Game::formMessages(std::deque<Response> &results) {
        std::deque<Message> outgoing;
        std::map<uintptr_t, std::ostringstream> clientMessages;

        for (const auto &entry : results) {
            if (entry.isLocal) {
                clientMessages[entry.clientId] << entry.message;

            } else {
                for (auto client : *this->clients) {
                    // Send to public messages to logged in users only
                    if (this->activeClientToId.count(client.id)) {
                        clientMessages[client.id] << entry.message;
                    }
                }
            }
        }

        for (auto const& [clientId, message] : clientMessages) {
            outgoing.push_back({{clientId}, message.str()});
        }

        return outgoing;
    }

    /* Login/Register Code */
    void
    Game::loginPlayer(uintptr_t clientId, IdType playerId) {
        this->activeIdToClient.emplace(playerId, clientId);
        this->activeClientToId.emplace(clientId, playerId);
    }

    void
    Game::logoutPlayer(IdType playerId) {
        this->activeClientToId.erase(this->activeIdToClient.at(playerId));
        this->activeIdToClient.erase(playerId);
    }
    /* End */

    std::deque<Message>
    Game::processCycle(std::deque<Message> &incoming) {
        std::deque<Response> results;

        this->handleConnects(results);

        this->handleDisconnects(results);

        this->handleIncoming(incoming, results);

        this->handleOutgoing(results);

        return this->formMessages(results);
    }
}
