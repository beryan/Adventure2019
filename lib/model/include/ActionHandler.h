//
// Created by louis on 19/01/19.
//

#ifndef WEBSOCKETNETWORKING_ACTIONHANDLER_H
#define WEBSOCKETNETWORKING_ACTIONHANDLER_H

#include "Server.h"
#include "ActionResult.h"

#include <deque>

using networking::Connection;
using networking::Message;

namespace model {
/**
 *  @class ActionHandler
 *
 *  @brief A class that generates outgoing messages based on user input
 *
 *  This class processes the actions a user makes in the game and stores
 *  a reference to functions that disconnect a user from the server or
 *  shuts down the server. These referenced functions are called when
 *  the "quit" or "shutdown" action is entered.
 */
    class ActionHandler {
    private:
        std::function<void(Connection action)> disconnect;
        std::function<void()> shutdown;

    public:
        ActionHandler(std::function<void(Connection)> &disconnect, std::function<void()> &shutdown);
        std::deque<ActionResult> processActions(std::deque<Message> incoming);
    };
}

#endif //WEBSOCKETNETWORKING_ACTIONHANDLER_H
