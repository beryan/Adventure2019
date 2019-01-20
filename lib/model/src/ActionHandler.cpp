//
// Created by louis on 19/01/19.
//

#include "Server.h"
#include "ActionHandler.h"
#include "Door.h"
#include "Room.h"

#include <sstream>
#include <iostream>

using model::ActionHandler;
using model::ActionResult;

std::string
lowercase(std::string string) {
    std::transform(string.begin(), string.end(), string.begin(), ::tolower);
    return string;
}

namespace model {
    ActionHandler::ActionHandler(std::function<void(Connection)> &disconnect, std::function<void()> &shutdown) {
        this->disconnect = disconnect;
        this->shutdown = shutdown;
    }

    std::deque<ActionResult>
    ActionHandler::processActions(std::deque<Message> incoming) {
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
}