//
// Created by louis on 19/01/19.
//

#ifndef WEBSOCKETNETWORKING_GAME_H
#define WEBSOCKETNETWORKING_GAME_H


#include <Server.h>
#include "ActionResult.h"

namespace model {
    class Game {
    private:
        networking::Server *server;
        std::vector<networking::Connection> *clients;
        bool *quit;

        std::deque<model::ActionResult> receive();

        void send(std::deque<model::ActionResult> results);

    public:
        Game(networking::Server &server, std::vector<networking::Connection> &clients, bool &quit);
        void processCycle();
    };
}

#endif //WEBSOCKETNETWORKING_GAME_H
