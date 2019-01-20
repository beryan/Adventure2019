//
// Created by louis on 19/01/19.
//

#include "Game.h"

#include <map>
#include <sstream>

using model::Game;

namespace model {
    Game::Game(std::vector<Connection> &clients) {
        this->clients = &clients;
    }

    void
    Game::setActionHandler(ActionHandler &actionHandler) {
        this->actionHandler = &actionHandler;
    }

    std::deque<ActionResult>
    Game::receive(std::deque<Message> incoming) {
        return this->actionHandler->processActions(incoming);
    }

    std::deque<Message>
    Game::send(std::deque<ActionResult> results) {
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
    Game::processCycle(std::deque<Message> incoming) {
        std::deque<ActionResult> results = this->receive(incoming);
        return this->send(results);
    }
}
