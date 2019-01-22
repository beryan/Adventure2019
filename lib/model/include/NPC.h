//
// Created by jnhkm on 2019-01-18.
//

#ifndef WEBSOCKETNETWORKING_NPC_H
#define WEBSOCKETNETWORKING_NPC_H

#include "User.h"

namespace model {
    class NPC : public User {
    public:
        NPC(int id) :
            User(std::move(id)),
            shortdesc({}),
            longdesc({}) {}

        NPC(int id, std::vector<std::string> keys, std::vector<std::string> desc, std::string shdesc, std::vector<std::string> lngdesc) :
                User(std::move(id),std::move(keys), std::move(desc)),
                shortdesc(std::move(shdesc)),
                longdesc(std::move(lngdesc)) {}

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