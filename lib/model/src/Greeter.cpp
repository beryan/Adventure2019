//
// Created by louis on 22/01/19.
//

#include "Greeter.h"

using model::Greeter;

namespace model {
    void
    Greeter::addClient(unsigned long int clientId) {
        this->clientIds.push_back(clientId);
    }

    void
    Greeter::processEvents(std::deque<model::GameResponse> &results) {
        while (!this->clientIds.empty()) {
            GameResponse result = GameResponse();
            result.setClientId(this->clientIds.back());
            result.setMessage("Welcome to Adventure 2019!\n");

            this->clientIds.pop_back();
            results.push_back(result);
        }
    }
}
