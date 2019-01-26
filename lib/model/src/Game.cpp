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
    Game::handleConnects(std::deque<Response> &responses) {
        while (!this->newClientIds->empty()) {
            auto clientId = this->newClientIds->back();

            std::ostringstream introduction;

            introduction << "Welcome to Adventure 2019!\n"
                         << "\n"
                         << "Enter \"login [username] [password]\" to sign into an existing account\n"
                         << "Enter \"register [username] [password]\" to make a new account\n";

            responses.push_back({clientId, introduction.str(), true});

            this->newClientIds->pop_back();
        }
    }

    void
    Game::handleDisconnects(std::deque<Response> &responses) {
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
    Game::handleIncoming(const std::deque<Message> &incoming, std::deque<Response> &responses) {
        for (const auto& input : incoming) {
            uintptr_t clientId = input.connection.id;

            std::string command = lowercase(input.text.substr(0, input.text.find(' ')));
            std::string parameters;

            if (input.text.find(' ') != std::string::npos) {
                parameters = input.text.substr(input.text.find(' ') + 1);
            }

            if (command == COMMAND_QUIT) {
                this->disconnect(input.connection);
                continue;

            } else if (command == COMMAND_SHUTDOWN) {
                std::cout << "Shutting down.\n";
                this->shutdown();
                return;
            }

            bool isLoggedIn = (bool) this->activeClientToId.count(clientId);
            Response response;

            if (isLoggedIn) {
                response = this->executeInGameAction(clientId, command, parameters);

            } else {
                response = this->executeMenuAction(clientId, command, parameters);
            }

            responses.push_back(response);
        }
    }

    Response
    Game::executeMenuAction(const uintptr_t &clientId, const std::string &command, const std::string &param) {
        std::ostringstream tempMessage;

        if (command == COMMAND_REGISTER) {
            /* Login/Register code */

            if (param.find(' ') == std::string::npos) {
                tempMessage << "Missing fields for registration.\n";
                return {clientId, tempMessage.str(), true};
            }

            std::string inputUsername = param.substr(0, param.find(' '));
            std::string inputPassword = param.substr(param.find(' ') + 1);

            if (this->usernameToPlayer.count(inputUsername)) {
                tempMessage << "The username " << inputUsername << " has already been taken,"
                            << " please use a different username.\n";
                return {clientId, tempMessage.str(), true};
            }

            int currentId = this->nextId;
            ++this->nextId;
            this->idToPlayer.emplace(currentId, Player(currentId, inputUsername, inputPassword));
            this->usernameToPlayer.emplace(inputUsername, &this->idToPlayer.at(currentId));
            this->loginPlayer(clientId, currentId);

            std::cout << inputUsername << " has been registered to the game\n";
            tempMessage << "Your account has been successfully registered and you are now logged in.\n";
            /* End */

        } else if (command == COMMAND_LOGIN) {
            /* Login/Register code */
            bool success = false;
            std::string inputUsername = param.substr(0, param.find(' '));

            if (param.find(' ') != std::string::npos && this->usernameToPlayer.count(inputUsername)) {

                std::string inputPassword = param.substr(param.find(' ') + 1);
                Player* selectedPlayer = this->usernameToPlayer.at(inputUsername);
                IdType playerId = selectedPlayer->getId();

                if ((selectedPlayer->getPassword() == inputPassword) && !this->activeIdToClient.count(playerId)) {
                    this->loginPlayer(clientId, playerId);

                } else {
                    // Player is already being used by a client, logout associated client
                    // and login with new client
                    uintptr_t oldClientId = this->activeIdToClient.at(playerId);
                    std::cout << oldClientId << "\n";

                    this->logoutPlayer(playerId);
                    this->loginPlayer(clientId, playerId);

                    std::cout << inputUsername << " is now being used by " << clientId << "\n";

                    return {
                        oldClientId,
                        "You have been logged out due to being logged in elsewhere.\n\n",
                        true
                    };
                }

                success = true;
            }

            if (success) {
                std::cout << inputUsername << " has entered the game\n";
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
                        << "  - " << COMMAND_REGISTER << " [username] [password] (create a new account with [username] [password])\n"
                        << "  - " << COMMAND_LOGIN    << " [username] [password] (login to an account with [username] [password])\n"
                        << "  - " << COMMAND_QUIT     << " (disconnects you from the game server)\n"
                        << "  - " << COMMAND_SHUTDOWN << " (shuts down the game server)\n"
                        << "\n";

        } else {
            tempMessage << "\n"
                        << "The word \"" << command << "\" is not a valid command.\n"
                        << "Enter " << "\"" << COMMAND_LOGIN << " [username] [password]\" to log into an existing account\n"
                        << "Enter " << "\"" << COMMAND_REGISTER << "[username] [password]\" to create a new account\n"
                        << "Enter " << "\"" << COMMAND_HELP << "\" for a full list of commands.\n";

        }

        return {clientId, tempMessage.str(), true};
    }

    Response
    Game::executeInGameAction(const uintptr_t &clientId, const std::string &command, const std::string &param) {
        std::ostringstream tempMessage;
        /* Login/Register code */
        IdType playerId = this->activeClientToId.at(clientId);
        /* End */
        bool isLocal = true;

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

        return {clientId, tempMessage.str(), isLocal};
    }


    void
    Game::handleOutgoing(std::deque<model::Response> &responses) {
        /*
         * Call handler methods that push Responses without requiring user input here
         */
    }

    std::deque<Message>
    Game::formMessages(std::deque<Response> &responses) {
        std::deque<Message> outgoing;
        std::map<uintptr_t, std::ostringstream> clientMessages;

        for (const auto &entry : responses) {
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
        this->activeClientToId.emplace(clientId, playerId);
        this->activeIdToClient.emplace(playerId, clientId);
    }

    void
    Game::logoutPlayer(IdType playerId) {
        this->activeClientToId.erase(this->activeIdToClient.at(playerId));
        this->activeIdToClient.erase(playerId);
    }
    /* End */

    std::deque<Message>
    Game::processCycle(std::deque<Message> &incoming) {
        std::deque<Response> responses;

        this->handleConnects(responses);
        this->handleDisconnects(responses);
        this->handleIncoming(incoming, responses);
        this->handleOutgoing(responses);

        return this->formMessages(responses);
    }
}
