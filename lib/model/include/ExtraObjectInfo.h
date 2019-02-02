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

    struct ExtraObjectInfo {
    public:
        ExtraObjectInfo();

        ExtraObjectInfo(
            std::vector<std::string> extraKeywords,
            std::vector<std::string> extraDescriptions
        );

        std::vector<std::string> getExtraKeywords() const;

        void setExtraKeywords(std::vector<std::string> extraKeywords);

        std::vector<std::string> getExtraDescriptions() const;

        void setExtraDescriptions(std::vector<std::string> extraDescriptions);

    private:
        std::vector<std::string> extraKeywords;

        std::vector<std::string> extraDescriptions;
    };
}

#endif //EXTRAOBJECTINFO_H