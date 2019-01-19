//
// Created by louis on 19/01/19.
//

#include <sstream>
#include <iostream>
#include "ActionHandler.h"
#include <Door.h>

using model::ActionHandler;

std::string
lowercase(std::string string) {
    std::transform(string.begin(), string.end(), string.begin(), ::tolower);
    return string;
}

namespace model {
    ActionHandler::ActionHandler(std::function<void(networking::Connection)> &disconnect, std::function<void()> &shutdown) {
        this->disconnect = disconnect;
        this->shutdown = shutdown;
    }

    std::deque<model::ActionResult>
    ActionHandler::processActions(std::deque<networking::Message> incoming) {
        std::deque<model::ActionResult> results;

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
                this->disconnect(action.connection);

            } else if (command == "shutdown") {
                std::cout << "Shutting down.\n";
                this->shutdown();

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
}