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
    const char* const Game::COMMAND_QUIT = "quit";
    const char* const Game::COMMAND_SAY = "say";
    const char* const Game::COMMAND_HELP = "help";
    const char* const Game::COMMAND_LOGOUT = "logout";
    const char* const Game::COMMAND_REGISTER = "register";
    const char* const Game::COMMAND_INFO = "info";
    const char* const Game::COMMAND_START = "start";

    Game::Game(std::vector<Connection> &clients,
               std::vector<unsigned long int> &newClientIds,
               std::vector<unsigned long int> &disconnectedClientIds,
               std::function<void(Connection)> &disconnect,
               std::function<void()> &shutdown) {
        this->clients = &clients;
        this->newClientIds = &newClientIds;
        this->disconnectedClientIds = &disconnectedClientIds;
        this->disconnect = disconnect;
        this->shutdown = shutdown;
        this->activePlayerList = {};
    }

    void
    Game::handleConnects(std::deque<GameResponse> &results) {
        while (!this->newClientIds->empty()) {
            auto clientId = this->newClientIds->back();

            std::cout << clientId << " has connected to the game.\n";
            results.emplace_back(GameResponse(clientId, "Welcome to Adventure 2019!\n"));

            this->newClientIds->pop_back();
        }
    }

    void
    Game::handleDisconnects(std::deque<GameResponse> &results) {
        while (!this->disconnectedClientIds->empty()) {
            auto clientId = this->disconnectedClientIds->back();

            std::cout << clientId << " has disconnected from the game.\n";

            this->disconnectedClientIds->pop_back();
        }
    }

    void
    Game::handleIncoming(const std::deque<Message> &incoming, std::deque<GameResponse> &results) {
        for (auto& input : incoming) {
            std::ostringstream tempMessage;
            std::string command = lowercase(input.text.substr(0, input.text.find(' ')));
            std::string param;
            bool isLocal = true;

            if (input.text.find(' ') != std::string::npos) {
                param = input.text.substr(input.text.find(' ') + 1);
            }

            if (command == COMMAND_QUIT) {
                this->disconnect(input.connection);

            } else if (command == COMMAND_SHUTDOWN) {
                std::cout << "Shutting down.\n";
                this->shutdown();

            } else if (command == COMMAND_START) {
                WorldHandler wh;

            } else if (command == COMMAND_SAY) {
                isLocal = false;
                tempMessage << input.connection.id << "> " << param << "\n";

            } else if (command == COMMAND_LOGOUT) {
                tempMessage << "Logout not yet implemented\n";

            } else if (command == COMMAND_REGISTER) {
                tempMessage << "Registering not yet implemented\n";

            } else if (command == COMMAND_HELP) {
                tempMessage << "\n"
                            << "********\n"
                            << "* HELP *\n"
                            << "********\n"
                            << "\n"
                            << "COMMANDS:\n"
                            << "  - " << COMMAND_HELP     << " (shows this help interface)\n"
                            << "  - " << COMMAND_SAY      << " [message] (sends [message] to other players in the game)\n"
                            << "  - " << COMMAND_REGISTER << " [email password](registers user [email] with password [password])\n"
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

            results.emplace_back(GameResponse(input.connection.id, tempMessage.str(), isLocal));
        }
    }

    void
    Game::handleOutgoing(std::deque<model::GameResponse> &results) {
        /*
         * Call handler methods that push GameResponses without requiring user input here
         */
    }

    std::deque<Message>
    Game::formMessages(std::deque<GameResponse> &results) {
        std::deque<Message> outgoing;
        std::map<unsigned long int, std::ostringstream> clientMessages;

        for (auto entry : results) {
            if (entry.isLocal()) {
                clientMessages[entry.getClientId()] << entry.getMessage();

            } else {
                for (auto client : *this->clients) {
                    clientMessages[client.id] << entry.getMessage();
                }
            }
        }

        for (auto const& [clientId, message] : clientMessages) {
            outgoing.push_back({clientId, message.str()});
        }

        return outgoing;
    }

    std::optional<Player>
    Game::getPlayer(const unsigned long int &clientId) {
        std::optional<Player> player;

        if (this->activePlayerList.count(clientId)) {
            player = this->activePlayerList.at(clientId);
        }

        return player;
    }

    std::deque<Message>
    Game::processCycle(std::deque<Message> &incoming) {
        std::deque<GameResponse> results;

        this->handleConnects(results);

        this->handleDisconnects(results);

        this->handleIncoming(incoming, results);

        this->handleOutgoing(results);

        return this->formMessages(results);
    }
}
