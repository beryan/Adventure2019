//
// Created by jnhkm on 2019-01-18.
//

#ifndef WEBSOCKETNETWORKING_NPC_H
#define WEBSOCKETNETWORKING_NPC_H

#include "User.h"

namespace model {
    class NPC : public User {
    public:
        NPC(int id);

        NPC(int id, std::vector<std::string> keys, std::vector<std::string> desc, std::string shdesc, std::vector<std::string> lngdesc);

        std::string getShortDescription();

        void setShortDescription(std::string desc);

        std::vector<std::string> getLongDescription();

        void addLongDescription(std::string desc);
    private:
        std::string shortdesc;

        std::vector<std::string> longdesc;

    };
}



#endif //WEBSOCKETNETWORKING_NPC_H
