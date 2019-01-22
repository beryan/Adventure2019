//
// Created by louis on 22/01/19.
//

#ifndef WEBSOCKETNETWORKING_GREETER_H
#define WEBSOCKETNETWORKING_GREETER_H

#include <deque>
#include <vector>
#include "GameResponse.h"

namespace model {
    /**
     * @class Greeter
     *
     * @brief A class for sending introduction text to new connected users.
     *
     */
    class Greeter {
    private:
        std::vector<unsigned long int> clientIds;

    public:
        /**
         * Adds a clientId to the Greeter's client id list.
         */
        void
        addClient(unsigned long int clientId);

        /**
         * Appends ActionResult objects containing a greeting to the Game's outgoing message list.
         */
        void
        processEvents(std::deque<model::GameResponse> &results);
    };
}

#endif //WEBSOCKETNETWORKING_GREETER_H
