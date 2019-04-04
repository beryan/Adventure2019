//
// Created by jnhkm on 2019-01-18.
//

#ifndef NPC_H
#define NPC_H

#include <vector>
#include "Character.h"
#include "json.hpp"

using Json = nlohmann::json;

namespace model {
    /**
    *  @class NPC
    *
    *  @brief A class describing an NPC.
    *
    *  The NPC (Non-Playing Character) class describes all the attributes of
    *  a specific type of character in the application.
    */

    class NPC : public Character {
    public:
        explicit NPC(model::ID id);

        NPC();

        NPC(
            model::ID id,
            std::string shortDescription
        );

        NPC(
            model::ID id,
            std::vector<std::string> keywords,
            std::vector<std::string> description,
            std::string shortDescription,
            std::vector<std::string> longDescription
        );

        model::ID getUniqueId() const;

        void setUniqueId(model::ID id);

        std::vector<std::string> getKeywords() const;

        void setKeywords(std::vector<std::string> keywords);

        std::vector<std::string> getDescription() const;

        void setDescription(std::vector<std::string> description);

        std::string getShortDescription() const;

        void setShortDescription(std::string shortDescription);

        std::vector<std::string> getLongDescription() const;

        void setLongDescription (std::vector<std::string> longDescription);

        void addToLongDescription(std::string addToLongDescription);

        bool containsKeyword(const std::string &keyword) const;

        static constexpr int MAX_KEYWORDS = 8;

    private:
        model::ID uniqueId = 0;

        std::vector<std::string> keywords;

        std::vector<std::string> description;

        std::string shortDescription;

        std::vector<std::string> longDescription;

        friend std::ostream& operator<<(std::ostream& os, const NPC& npc);
    };

    inline void from_json(const Json &json, NPC &npc) {
        npc.setId(json.at("id").get<model::ID>());
        npc.setKeywords(json.at("keywords").get<std::vector<std::string>>());
        npc.setDescription(json.at("description").get<std::vector<std::string>>());
        npc.setLongDescription(json.at("longdesc").get<std::vector<std::string>>());
        npc.setShortDescription(json.at("shortdesc").get<std::string>());
    }

    inline void to_json(Json &json, const NPC &npc) {
        json = {
                {"description", npc.getDescription()},
                {"keywords", npc.getKeywords()},
                {"id", npc.getId()},
                {"longdesc", npc.getLongDescription()},
                {"shortdesc", npc.getShortDescription()}
        };
    }

}

#endif //NPC_H