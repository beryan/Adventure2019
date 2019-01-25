//
// Created by jnhkm on 2019-01-18.
//

#include "NPC.h"


namespace model {

    NPC::NPC(int id) :
        Character(id),
        keywords({}),
        description({}),
        shortDescription({}),
        longDescription({})
        {}

    NPC::NPC(
        int id,
        std::vector<std::string> keywords,
        std::string description,
        std::string shortDescription,
        std::vector<std::string> longDescription
    ) :
        Character(id),
        keywords(std::move(keywords)),
        description(std::move(description)),
        shortDescription(std::move(shortDescription)),
        longDescription(std::move(longDescription))
        {}

    std::string NPC::getDescription() const {
        return this->description;
    }

    void NPC::setDescription(std::string description) {
        this->description = std::move(description);
    }

    std::vector<std::string> NPC::getKeywords() const {
        return this->keywords;
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
}