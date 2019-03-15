//
// Created by Waswa Olunga on 2019-01-31.
//

#include <string>
#include <vector>
#include <algorithm>
#include "ExtraObjectInfo.h"

namespace model {

    ExtraInfo::ExtraInfo() :
            extraKeywords({}),
            extraDescriptions({}) {}

    ExtraInfo::ExtraInfo(
            std::vector<std::string> extraKeywords,
            std::vector<std::string> extraDescriptions
    ) :
            extraKeywords(std::move(extraKeywords)),
            extraDescriptions(std::move(extraDescriptions)) {}

    std::vector<std::string> ExtraInfo::getExtraKeywords() const {
        return this->extraKeywords;
    }

    void ExtraInfo::setExtraKeywords(std::vector<std::string> extraKeywords) {
        this->extraKeywords = std::move(extraKeywords);
    }

    std::vector<std::string> ExtraInfo::getExtraDescriptions() const {
        return this->extraDescriptions;
    }

    void ExtraInfo::setExtraDescriptions(std::vector<std::string> extraDescriptions) {
        this->extraDescriptions = std::move(extraDescriptions);
    }

    bool ExtraObjectInfo::containsKeyword(const std::string &keyword) const {
        return (std::find(this->extraKeywords.begin(), this->extraKeywords.end(), keyword) != this->extraKeywords.end());
    }

    bool ExtraObjectInfo::operator==(const ExtraObjectInfo &other) const {
        return this->getExtraKeywords() == other.getExtraKeywords() &&
               this->getExtraDescriptions() == other.getExtraDescriptions();
    }

    ExtraInfo& ExtraInfo::operator=(const ExtraInfo &other){
        this->extraKeywords = other.getExtraKeywords();
        this->extraDescriptions = other.getExtraDescriptions();

        return *this;
    }

    bool ExtraInfo::operator!=(const ExtraInfo &other) const {
        return !(*this == other);
    }

}
