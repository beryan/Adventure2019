//
// Created by Waswa Olunga on 2019-01-18.
//

#include <iostream>
#include <Object.h>

namespace model {

    Object::Object() :
            id(Object::DEFAULT_ID),
            shortDescription({}),
            longDescription({}),
            keywords({}),
            slot(Slot::Misc),
            extraObjectInfo({}) {}

    Object::Object(
            model::ID id,
            std::string shortDescription,
            std::vector<std::string> longDescription,
            std::vector<std::string> keywords,
            Slot slot
    ) :
            id(id),
            shortDescription(std::move(shortDescription)),
            longDescription(std::move(longDescription)),
            keywords(std::move(keywords)),
            slot(slot),
            extraObjectInfo({}) {}

    Object::Object(
            model::ID id,
            std::string shortDescription,
            std::vector<std::string> longDescription,
            std::vector<std::string> keywords,
            Slot slot,
            std::vector<ExtraInfo> extraObjectInfo
    ) :
            id(id),
            shortDescription(std::move(shortDescription)),
            longDescription(std::move(longDescription)),
            keywords(std::move(keywords)),
            slot(slot),
            extraObjectInfo(std::move(extraObjectInfo)) {}

    model::ID Object::getId() const {
        return this->id;
    }

    void Object::setId(model::ID id) {
        this->id = id;
    }

    std::string Object::getShortDescription() const {
        return this->shortDescription;
    }

    void Object::setShortDescription(std::string shortDescription) {
        this->shortDescription = std::move(shortDescription);
    }

    std::vector<std::string> Object::getLongDescription() const {
        return this->longDescription;
    }

    void Object::setLongDescription(std::vector<std::string> longDescription) {
        this->longDescription = std::move(longDescription);
    }

    std::vector<std::string> Object::getKeywords() const {
        return this->keywords;
    }

    void Object::setKeywords(std::vector<std::string> keywords) {
        this->keywords = std::move(keywords);
    }


    Slot Object::getSlot() const {
        return this->slot;
    }

    void Object::setSlot(model::Slot slot) {
        this->slot = slot;
    }

    std::vector<ExtraInfo> Object::getExtraObjectInfo() const {
        return extraObjectInfo;
    }

    void Object::setExtraObjectInfo(std::vector<ExtraInfo> extraObjectInfo) {
        this->extraObjectInfo = std::move(extraObjectInfo);
    }

    bool Object::canBeEquipped() const {
        return this->slot >= 0 && this->slot < Slot::Misc;
    }

    bool Object::containsKeyword(const std::string &keyword) const {
        return (std::find(this->keywords.begin(), this->keywords.end(), keyword) != this->keywords.end());
    }

    bool Object::operator==(const Object &object) const {
        return  this->id == object.getId();
    }

    bool Object::operator<(const Object &object) const {
        return (this->id < object.getId());
    }

    Object& Object::operator=(const Object &object) {
        this->id = object.getId();
        this->shortDescription = object.getShortDescription();
        this->longDescription = object.getLongDescription();
        this->keywords = object.getKeywords();
        this->slot = object.getSlot();
        this->extraObjectInfo = object.getExtraObjectInfo();

        return *this;
    }

    std::ostream&operator<<(std::ostream& os, const Object& obj) {
        os << obj.id << ". " << obj.shortDescription << std::endl;
        return os;
    }
}
