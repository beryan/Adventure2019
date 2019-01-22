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
               std::function<void()> &shutdown
               ) {
        this->clients = &clients;
        this->newClientIds = &newClientIds;
        this->disconnectedClientIds = &disconnectedClientIds;
        this->disconnect = disconnect;
        this->shutdown = shutdown;

        this->greeter = std::make_unique<Greeter>();
    }

    std::deque<GameResponse>
    Game::handleIncoming(const std::deque<Message> &incoming) {
        std::deque<GameResponse> results;

        for (auto& action : incoming) {
            auto result = GameResponse();
            result.setClientId(action.connection.id);

            std::ostringstream tempMessage;
            std::string command = lowercase(action.text.substr(0, action.text.find(' ')));
            std::string param;

            if (action.text.find(' ') != std::string::npos) {
                param = action.text.substr(action.text.find(' ') + 1);
            }

            if (command == "quit") {
                this->disconnect(action.connection);

            } else if (command == "shutdown") {
                std::cout << "Shutting down.\n";
                this->shutdown();

            } else if (command == "start") {
                WorldHandler wh;

            } else if (command == "say") {
                result.setPublic();
                tempMessage << action.connection.id << "> " << param << "\n";

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

            result.setMessage(tempMessage.str());
            results.push_back(result);
        }

        return results;
    }

    void
    Game::handleEvents(std::deque<model::GameResponse> &results) {
        this->greeter->processEvents(results);
    }

    std::deque<Message>
    Game::handleOutgoing(std::deque<GameResponse> &results) {
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

    void
    Game::handleConnects() {
        while (!this->newClientIds->empty()) {
            auto clientId = this->newClientIds->back();

            this->greeter->addClient(clientId);

            this->newClientIds->pop_back();
        }
    }

    void
    Game::handleDisconnects() {
        while (!this->disconnectedClientIds->empty()) {
            auto clientId = this->disconnectedClientIds->back();

            /*
             * call handler methods here
             */

            this->disconnectedClientIds->pop_back();
        }
    }

    std::deque<Message>
    Game::processCycle(std::deque<Message> &incoming) {
        this->handleConnects();

        this->handleDisconnects();

        std::deque<GameResponse> results = this->handleIncoming(incoming);

        this->handleEvents(results);

        return this->handleOutgoing(results);
    }
}
