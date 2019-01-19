//
// Created by jnhkm on 2019-01-18.
//

#include "NPC.h"

using model::NPC;

namespace model {
    std::string NPC::getShortDescription() {return this->shortdesc;};

    void NPC::setShortDescription(std::string desc) {shortdesc.assign(desc);};

    std::vector<std::string> NPC::getLongDescription() {return this->longdesc;};

    void NPC::addLongDescription(std::string desc) {longdesc.push_back(std::move(desc));};
}