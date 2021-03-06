//
// Created by jnhkm on 2019-01-18.
//

#include "NPC.h"
#include "json.hpp"
#include <iostream>
#include <sstream>
#include <boost/algorithm/string/join.hpp>

using Json = nlohmann::json;

namespace model {

    NPC::NPC() :
        Character(0),
        keywords({}),
        description({}),
        shortDescription({}),
        longDescription({})
        {}
    
    NPC::NPC(model::ID id) :
        Character(id),
        keywords({}),
        description({}),
        shortDescription({}),
        longDescription({})
        {}

    NPC::NPC(model::ID id, std::string shortDescription) :
        Character(id),
        keywords({}),
        description({}),
        shortDescription(std::move(shortDescription)),
        longDescription({})
        {}

    NPC::NPC(
        model::ID id,
        std::vector<std::string> keywords,
        std::vector<std::string>description,
        std::string shortDescription,
        std::vector<std::string> longDescription
    ) :
        Character(id),
        keywords(std::move(keywords)),
        description(std::move(description)),
        shortDescription(std::move(shortDescription)),
        longDescription(std::move(longDescription))
        {}

    NPC::NPC(
        model::ID id,
        model::ID uniqueId,
        std::vector<std::string> keywords,
        std::vector<std::string>description,
        std::string shortDescription,
        std::vector<std::string> longDescription
    ) :
        Character(id),
        uniqueId(uniqueId),
        keywords(std::move(keywords)),
        description(std::move(description)),
        shortDescription(std::move(shortDescription)),
        longDescription(std::move(longDescription))
        {}

    std::vector<std::string> NPC::getDescription() const {
        return this->description;
    }

    void NPC::setDescription(std::vector<std::string>description) {
        this->description = std::move(description);
    }

    std::vector<std::string> NPC::getKeywords() const {
        return this->keywords;
    }

    model::ID NPC::getUniqueId() const {
        return this->uniqueId;
    }

    void NPC::setUniqueId(model::ID id) {
        this->uniqueId = id;
    }

    void NPC::setKeywords(std::vector<std::string> keywords) {

        if (keywords.size() > NPC::MAX_KEYWORDS) {
            keywords.erase(keywords.begin() + NPC::MAX_KEYWORDS, keywords.end());
        }

        this->keywords = std::move(keywords);
    }

    std::string NPC::getShortDescription() const {
        return this->shortDescription;
    }

    void NPC::setShortDescription(std::string shortDescription) {
        this->shortDescription = std::move(shortDescription);
    }

    std::vector<std::string> NPC::getLongDescription() const {
        return this->longDescription;
    }

    void NPC::setLongDescription (std::vector<std::string> longDescription) {
        this->longDescription = std::move(longDescription);
    }

    void NPC::addToLongDescription(std::string addToLongDescription) {
        this->longDescription.push_back(std::move(addToLongDescription));
    }

    bool NPC::containsKeyword(const std::string &keyword) const {
        return std::find(this->keywords.begin(), this->keywords.end(), keyword) != this->keywords.end();
    }

    //print npc
    std::ostream& operator<<(std::ostream& os, const NPC& npc) {
        os << "\nNPC State\n";
        os << "---------\n";
        os << "Id: [" << npc.getId() << "]\n";
        auto keywords = boost::algorithm::join(npc.keywords, ", ");
        os << "Keywords: [" << keywords << "]\n";
        os << "Short: [" << npc.shortDescription << "]\n";
        auto longDesc = boost::algorithm::join(npc.longDescription, "\n");
        os << "Long: [" << longDesc << "]\n";
        auto desc = boost::algorithm::join(npc.description, "\n");
        os << "Desc: [" << desc << "]\n";
        return os;
    }
}
