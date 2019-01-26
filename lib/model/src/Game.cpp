//
// Created by louis on 19/01/19.
//

#include "Game.h"
#include "Room.h"
#include "PlayerHandler.h"

#include <map>
#include <sstream>
#include <iostream>

using model::Game;
using model::Room;
using model::PlayerHandler;

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

        this->playerHandler = std::make_unique<PlayerHandler>();
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

            if (this->playerHandler->isLoggedIn(clientId)) {
                this->playerHandler->logoutPlayer(clientId);
                std::cout << clientId << " has been logged out of the game due to disconnect\n";
            }

            this->disconnectedClientIds->pop_back();
        }
    }

    void
    Game::handleIncoming(const std::deque<Message> &incoming, std::deque<Response> &responses) {
        for (const auto& input : incoming) {
            auto clientId = input.connection.id;

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

            if (!this->playerHandler->isLoggedIn(clientId)) {
                responses.push_back(this->executeMenuAction(clientId, command, parameters));

            } else {
                responses.push_back(this->executeInGameAction(clientId, command, parameters));
            }
        }
    }

    Response
    Game::executeMenuAction(const uintptr_t &clientId, const std::string &command, const std::string &param) {
        std::ostringstream tempMessage;

        if (command == COMMAND_REGISTER) {
            tempMessage << this->playerHandler->registerPlayer(clientId, param);

        } else if (command == COMMAND_LOGIN) {
            tempMessage << this->playerHandler->loginPlayer(clientId, param);

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
        bool isLocal = true;

        if (command == COMMAND_SAY) {
            isLocal = false;
            tempMessage << this->playerHandler->getUsernameByClientId(clientId) << "> " << param << "\n";

        } else if (command == COMMAND_LOGOUT) {
            tempMessage << this->playerHandler->logoutPlayer(clientId);

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
    Game::handleOutgoing(std::deque<Response> &responses) {

        this->playerHandler->notifyBootedClients(responses);

    }

    std::deque<Message>
    Game::formMessages(std::deque<Response> &responses) {
        std::deque<Message> outgoing;
        std::map<uintptr_t, std::ostringstream> clientMessages;

        for (const auto &entry : responses) {
            if (entry.isLocal) {
                clientMessages[entry.clientId] << entry.message;

            } else {
                for (const auto &client : *this->clients) {
                    // Send to public messages to logged in users only
                    if (this->playerHandler->isLoggedIn(client.id)) {
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
