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

            if (command == "quit") {
                this->disconnect(input.connection);

            } else if (command == "shutdown") {
                std::cout << "Shutting down.\n";
                this->shutdown();

            } else if (command == "start") {
                WorldHandler wh;

            } else if (command == "say") {
                isLocal = false;
                tempMessage << input.connection.id << "> " << param << "\n";

            } else if (command == "help") {
                tempMessage << "********\n"
                            << "* HELP *\n"
                            << "********\n"
                            << "\n"
                            << "COMMANDS:\n"
                            << "  - help (shows this help interface)\n"
                            << "  - say [message] (sends [message] to other players in the game)\n"
                            << "  - quit (disconnects you from the game server)\n"
                            << "  - shutdown (shuts down the game server)\n"
                            << "  - info (displays current location information)\n"
                            << "\n";

            } else if (command == "info") {
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
