//
// Created by Waswa Olunga on 2019-01-31.
//

#ifndef EXTRAOBJECTINFO_H
#define EXTRAOBJECTINFO_H

#include <string>
#include <vector>

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

        void setExtraDescriptions(std::vector<std::string> extraDescriptions);

        bool containsKeyword(const std::string &keyword) const;

        bool operator==(const ExtraObjectInfo &other) const;

        ExtraInfo& operator=(const ExtraInfo &other);

        bool operator!=(const ExtraInfo &other) const;

    private:
        std::vector<std::string> extraKeywords;

        std::vector<std::string> extraDescriptions;
    };
}

#endif //EXTRAOBJECTINFO_H
