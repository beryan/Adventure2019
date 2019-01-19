//
// Created by louis on 19/01/19.
//

#include <Game.h>
#include <Door.h>
#include <iostream>
#include <deque>
#include "ActionResult.h"
#include <sstream>
#include <map>
#include <Server.h>

using model::Game;

std::string
lowercase(std::string string) {
std::transform(string.begin(), string.end(), string.begin(), ::tolower);
return string;
}

namespace model {
    Game::Game(networking::Server &server, std::vector<networking::Connection> &clients, bool &quit) {
        this->server = &server;
        this->clients = &clients;
        this->quit = &quit;
    }

    std::deque<model::ActionResult>
    Game::receive() {
        std::deque<model::ActionResult> results;
        auto incoming = this->server->receive();

        for (auto& action : incoming) {
            auto result = model::ActionResult();
            result.setClientId(action.connection.id);

            std::ostringstream tempMessage;
            std::string command = lowercase(action.text.substr(0, action.text.find(' ')));
            std::string param;

            if (action.text.find(' ') != std::string::npos) {
                param = action.text.substr(action.text.find(' ') + 1);
            }

            if (command == "quit") {
                this->server->disconnect(action.connection);

            } else if (command == "shutdown") {
                std::cout << "Shutting down.\n";
                *this->quit = true;

            } else if (command == "say") {
                result.setPublic();
                tempMessage << action.connection.id << "> " << param << "\n";

            } else if (command == "help") {
                tempMessage << "********\n";
                tempMessage << "* HELP *\n";
                tempMessage << "********\n";
                tempMessage << "\n";
                tempMessage << "COMMANDS:\n";
                tempMessage << "  - help (shows this help interface)\n";
                tempMessage << "  - say [message] (sends [message] to other players in the game)\n";
                tempMessage << "  - quit (disconnects you from the game server)\n";
                tempMessage << "  - shutdown (shuts down the game server)\n";
                tempMessage << "  - door (displays door information)\n";

            } else if (command == "door") {
                model::Door stubDoor1 = model::Door("north", 8801);
                model::Door stubDoor2 = model::Door("south", {"The door is old", "You can hear it creak"}, {"old","creak"}, 8855);
                tempMessage << stubDoor1 << std::endl;
                tempMessage << stubDoor2 << std::endl;
            } else {
                tempMessage << "The word \"" << command << "\" is not a valid command. Enter \"help\" for a list of commands.\n";
            }

            result.setMessage(tempMessage.str());
            results.push_back(result);
        }

        return results;
    }

    void
    Game::send(std::deque<model::ActionResult> results) {
        std::deque<networking::Message> outgoing;
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

        this->server->send(outgoing);
    }

    void
    Game::processCycle() {
        std::deque<model::ActionResult> results = this->receive();
        this->send(results);
    }
}
