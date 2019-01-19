//
// Created by louis on 15/01/19.
//

#ifndef WEBSOCKETNETWORKING_MESSAGERESULT_H
#define WEBSOCKETNETWORKING_MESSAGERESULT_H

#include <string>

namespace model {
    class ActionResult {
    private:
        unsigned long int clientId;
        std::string message;
        bool local = true;

    public:
        unsigned long int getClientId();

        void setClientId(unsigned long int clientId);

        std::string getMessage();

        void setMessage(std::string message);

        void setPublic();

        bool isLocal();
    };
}

#endif //WEBSOCKETNETWORKING_MESSAGERESULT_H
