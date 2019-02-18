//
// Created by jnhkm on 2019-01-18.
//

#ifndef NPC_H
#define NPC_H

#include <vector>
#include "Character.h"
#include "json.hpp"

using json = nlohmann::json;

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
            std::vector<std::string> keywords,
            std::string description,
            std::string shortDescription,
            std::vector<std::string> longDescription
        );

        std::vector<std::string> getKeywords() const;

        void setKeywords(std::vector<std::string> keywords);

        std::string getDescription() const;

        void setDescription(std::string description);

        std::string getShortDescription() const;

        void setShortDescription(std::string shortDescription);

        std::vector<std::string> getLongDescription() const;

        void setLongDescription (std::vector<std::string> longDescription);

        void addToLongDescription(std::string addToLongDescription);

        static constexpr int MAX_KEYWORDS = 8;

    private:
        std::vector<std::string> keywords;

        std::string description;

        std::string shortDescription;

        std::vector<std::string> longDescription;

        friend std::ostream& operator<<(std::ostream& os, const NPC& npc);
    };

    inline void from_json(const json &j, NPC &n) {
        n.setId(j.at("id").get<model::ID>());
        n.setKeywords(j.at("keywords").get<std::vector<std::string>>());
        n.setDescription(j.at("description").get<std::vector<std::string>>());
        n.setLongDescription(j.at("longdesc").get<std::vector<std::string>>());
        n.setShortDescription(j.at("shortdesc").get<std::string>());
    }
}

#endif //NPC_H