//
// Created by Waswa Olunga on 2019-01-31.
//

#include <string>
#include <vector>
#include <ExtraObjectInfo.h>

namespace model {

    ExtraObjectInfo::ExtraObjectInfo() :
            extraKeywords({}),
            extraDescriptions({}) {}

    ExtraObjectInfo::ExtraObjectInfo(
            std::vector<std::string> extraKeywords,
            std::vector<std::string> extraDescriptions
    ) :
            extraKeywords(std::move(extraKeywords)),
            extraDescriptions(std::move(extraDescriptions)) {}

    std::vector<std::string> ExtraObjectInfo::getExtraKeywords() const {
        return this->extraKeywords;
    }

    void ExtraObjectInfo::setExtraKeywords(std::vector<std::string> extraKeywords) {
        this->extraKeywords = std::move(extraKeywords);
    }

    std::vector<std::string> ExtraObjectInfo::getExtraDescriptions() const {
        return this->extraDescriptions;
    }

    void ExtraObjectInfo::setExtraDescriptions(std::vector<std::string> extraDescriptions) {
        this->extraDescriptions = std::move(extraDescriptions);
    }

    bool ExtraObjectInfo::operator==(const ExtraObjectInfo &other) const {
        return this->getExtraKeywords() == other.getExtraKeywords() &&
               this->getExtraDescriptions() == other.getExtraDescriptions();
    }

}