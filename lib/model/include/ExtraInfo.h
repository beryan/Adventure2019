//
// Created by Waswa Olunga on 2019-01-31.
//

#ifndef EXTRAINFO_H
#define EXTRAINFO_H

#include <string>
#include <vector>
#include "json.hpp"

using Json = nlohmann::json;

namespace model {

    /**
     *  @struct ExtraInfo
     *
     *  @brief A struct describing the extra information an object can have.
     *
     *  The ExtraInfo contains extra object info
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


    inline void from_json(const Json &json, ExtraInfo &extraInfo) {
        extraInfo.setExtraDescriptions(json.at("desc").get<std::vector<std::string>>());
        extraInfo.setExtraKeywords(json.at("keywords").get<std::vector<std::string>>());
    }

    inline void to_json(Json &json, const ExtraInfo &extraInfo) {
        json = {
                {"desc", extraInfo.getExtraDescriptions()},
                {"keywords", extraInfo.getExtraKeywords()}
        };
    }

}

#endif //EXTRAINFO_H
