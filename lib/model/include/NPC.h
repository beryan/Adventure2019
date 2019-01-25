//
// Created by jnhkm on 2019-01-18.
//

#ifndef NPC_H
#define NPC_H

#include <vector>
#include "Character.h"

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
        explicit NPC(int id);

        NPC(
            int id,
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
    };
}

#endif //NPC_H