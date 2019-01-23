//
// Created by louis on 19/01/19.
//

#include "GameResponse.h"

using model::GameResponse;

namespace model {
    GameResponse::GameResponse() {
        this->local = true;
    }

    GameResponse::GameResponse(unsigned long int clientId, std::string message) {
        this->clientId = clientId;
        this->message = std::move(message);
        this->local = true;
    }
    
    GameResponse::GameResponse(unsigned long int clientId, std::string message, bool isLocal) {
        this->clientId = clientId;
        this->message = std::move(message);
        this->local = isLocal;
    }
;
    unsigned long int
    GameResponse::getClientId() {
        return this->clientId;
    };

    void
    GameResponse::setClientId(unsigned long int clientId) {
        this->clientId = clientId;
    }

    std::string
    GameResponse::getMessage() {
        return this->message;
    }

    void
    GameResponse::setMessage(std::string message) {
        this->message = std::move(message);
    }

    void
    GameResponse::setPublic() {
        this->local = false;
    }

    bool
    GameResponse::isLocal() {
        return local;
    }

}

