//
// Created by louis on 19/01/19.
//

#include <Game.h>
#include <iostream>
#include <deque>
#include <sstream>
#include <map>
#include <Server.h>

using model::Game;
using model::ActionHandler;
using model::ActionResult;

namespace model {
    Game::Game(networking::Server &server, std::vector<networking::Connection> &clients, bool &quit) {
        this->server = &server;
        this->clients = &clients;
        this->quit = &quit;

        std::function<void(networking::Connection)> disconnect = [&server](networking::Connection connection) {
           server.disconnect(connection);
        };

        std::function<void()> shutdown = [&quit]() {
            quit = true;
        };

        this->setActionHandler(model::ActionHandler(disconnect, shutdown));
    }

    void
    Game::setActionHandler(model::ActionHandler actionHandler) {
        this->actionHandler = &actionHandler;
    }

    std::deque<model::ActionResult>
    Game::receive() {
        std::deque<model::ActionResult> results = this->actionHandler->processActions(this->server->receive());

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
