//
// Created by louis on 19/01/19.
//

#ifndef WEBSOCKETNETWORKING_ACTIONHANDLER_H
#define WEBSOCKETNETWORKING_ACTIONHANDLER_H

#include <deque>
#include <Server.h>
#include "ActionResult.h"

namespace model {
    class ActionHandler {
    private:
        std::function<void(networking::Connection action)> disconnect;
        std::function<void()> shutdown;

    public:
        ActionHandler(std::function<void(networking::Connection)> &disconnect, std::function<void()> &shutdown);
        std::deque<model::ActionResult> processActions(std::deque<networking::Message> incoming);
    };
}

#endif //WEBSOCKETNETWORKING_ACTIONHANDLER_H
