//
// Created by louis on 19/01/19.
//

#ifndef WEBSOCKETNETWORKING_GAME_H
#define WEBSOCKETNETWORKING_GAME_H

#include "ActionResult.h"
#include "ActionHandler.h"

namespace model {
/**
 * @class
 *
 * @brief A class for containing the game's logic
 *
 * This class manages the components necessary to gather, process, and output
 * information that is to be exchanged between the server and clients.
 *
 */
    class Game {
    private:
        std::vector<Connection> *clients;
        ActionHandler *actionHandler;

        std::deque<ActionResult>
        receive(std::deque<Message> incoming);

        std::deque<Message>
        send(std::deque<ActionResult> outgoing);

    public:
        explicit Game(std::vector<Connection> &clients);

        void
        setActionHandler(ActionHandler &actionHandler);

        std::deque<Message>
        processCycle(std::deque<Message>);
    };
}

#endif //WEBSOCKETNETWORKING_GAME_H
