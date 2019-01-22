//
// Created by jnhkm on 2019-01-18.
//

#include "NPC.h"

using model::NPC;

namespace model {
    NPC::NPC(int id) :
            User(id),
            shortdesc({}),
            longdesc({})
            {}

    NPC::NPC(int id, std::vector<std::string> keys, std::vector<std::string> desc, std::string shdesc, std::vector<std::string> lngdesc) :
            User(id, std::move(keys), std::move(desc)),
            shortdesc(std::move(shdesc)),
            longdesc(std::move(lngdesc))
            {}


    std::string NPC::getShortDescription() {
        return shortdesc;
    }

    void NPC::setShortDescription(std::string desc) {
        shortdesc.assign(desc);
    }

    std::vector<std::string> NPC::getLongDescription() {
        return longdesc;
    }

    void NPC::addLongDescription(std::string desc) {
        longdesc.push_back(std::move(desc));
    }
}