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
    Game::Game(std::vector<Connection> &clients, std::function<void(Connection)> &disconnect, std::function<void()> &shutdown) {
        this->clients = &clients;
        this->disconnect = disconnect;
        this->shutdown = shutdown;
    }

    std::deque<ActionResult>
    Game::handleIncoming(std::deque<Message> incoming) {
        std::deque<ActionResult> results;

        for (auto& action : incoming) {
            auto result = ActionResult();
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
    Game::handleEvents(std::deque<model::ActionResult> &results) {
        /*
         *  Event calls go here (events are not dependent on user input for sending responses);
         *
         *  e.g.
         *  ExampleHandler.processCycle(results)
         *
         *  where the handler's processCycle() performs:
         *    ActionResult result = ActionResult()
         *    result.setClientId(id)
         *    result.setMessage(message)
         *
         *    results.push_back(result)
         */
    }

    std::deque<Message>
    Game::handleOutgoing(std::deque<ActionResult> results) {
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
        std::deque<ActionResult> results = this->handleIncoming(incoming);

        this->handleEvents(results);

        return this->handleOutgoing(results);
    }
}
