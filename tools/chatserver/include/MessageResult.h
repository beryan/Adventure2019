//
// Created by louis on 15/01/19.
//

#ifndef WEBSOCKETNETWORKING_MESSAGERESULT_H
#define WEBSOCKETNETWORKING_MESSAGERESULT_H

#include <boost/beast/core/string.hpp>

class MessageResult {
private:
    unsigned long int clientId;
    std::string message;
    bool local = true;

public:
    unsigned long int getClientId() {
        return this->clientId;
    }

    void setClientId(unsigned long int clientId) {
        this->clientId = clientId;
    }

    std::string getMessage() {
        return this->message;
    }

    void setMessage(std::string message) {
        this->message = message;
    }

    void setPublic() {
        this->local = false;
    }

    bool isLocal() {
        return local;
    }
};

#endif //WEBSOCKETNETWORKING_MESSAGERESULT_H
