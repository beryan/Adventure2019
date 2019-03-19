//
// Created by Waswa Olunga on 2019-01-31.
//

#ifndef EXTRAOBJECTINFO_H
#define EXTRAOBJECTINFO_H

#include <string>
#include <vector>
#include "json.hpp"

using json = nlohmann::json;

namespace model {

    /**
     *  @struct ExtraObjectInfo
     *
     *  @brief A struct describing the extra information an object can have.
     *
     *  The ExtraObjectInfo contains extra object info
     *  including keywords and description
     */

    struct ExtraInfo {
    public:
        ExtraInfo();

        ExtraInfo(
                std::vector<std::string> extraKeywords,
                std::vector<std::string> extraDescriptions
        );

        std::vector<std::string> getExtraKeywords() const;

        void setExtraKeywords(std::vector<std::string> extraKeywords);

        std::vector<std::string> getExtraDescriptions() const;

        bool containsKeyword(const std::string &keyword) const;

        void setExtraDescriptions(std::vector<std::string> extraDescriptions);

        bool operator==(const ExtraInfo &other) const;

        ExtraInfo& operator=(const ExtraInfo &other);

        bool operator!=(const ExtraInfo &other) const;

    private:
        std::vector<std::string> extraKeywords;

        std::vector<std::string> extraDescriptions;
    };


    inline void from_json(const json &j, ExtraInfo &e) {
        e.setExtraDescriptions(j.at("desc").get<std::vector<std::string>>());
        e.setExtraKeywords(j.at("keywords").get<std::vector<std::string>>());
    }


}

#endif //EXTRAOBJECTINFO_H
