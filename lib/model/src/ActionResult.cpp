//
// Created by louis on 19/01/19.
//

#include <ActionResult.h>
#include <string>

using model::ActionResult;

namespace model {
    unsigned long int ActionResult::getClientId() {
        return this->clientId;
    };

    void ActionResult::setClientId(unsigned long int clientId) {
        this->clientId = clientId;
    }

    std::string ActionResult::getMessage() {
        return this->message;
    }

    void ActionResult::setMessage(std::string message) {
        this->message = message;
    }

    void ActionResult::setPublic() {
        this->local = false;
    }

    bool ActionResult::isLocal() {
        return local;
    }

}

