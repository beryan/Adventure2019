//
// Created by louis on 19/01/19.
//

#ifndef WEBSOCKETNETWORKING_GAME_H
#define WEBSOCKETNETWORKING_GAME_H

#include "ActionResult.h"
#include "Server.h"

#include <functional>
#include <deque>

using networking::Connection;
using networking::Message;

namespace model {
/**
 * @class Game
 *
 * @brief A class for containing the game's logic
 *
 * This class manages the components necessary to gather, process, and output
 * information that is to be exchanged between the server and clients.
 *
 */
    class Game {
    private:
        std::vector<Connection> clients;
        std::function<void(Connection action)> disconnect;
        std::function<void()> shutdown;

        std::deque<ActionResult>
        handleIncoming(std::deque<Message> incoming);

        void
        handleEvents(std::deque<ActionResult> &results);

        std::deque<Message>
        handleOutgoing(std::deque<ActionResult> outgoing);

    public:
        Game(std::vector<Connection> &clients, std::function<void(Connection)> &disconnect, std::function<void()> &shutdown);

        std::deque<Message>
        processCycle(std::deque<Message> &incoming);
    };
}

#endif //WEBSOCKETNETWORKING_GAME_H
