//
// Created by Waswa Olunga on 2019-01-18.
//

#include <Object.h>

namespace model {

    Object::Object() :
            id(Object::DEFAULT_ID),
            name({}),
            shortDescription({}),
            longDescription({}),
            keywords({}),
            slot(Slot::Misc),
            extraObjectInfo({}) {}

    Object::Object(
            model::ID id,
            std::string name,
            std::string shortDescription,
            std::vector<std::string> longDescription,
            std::vector<std::string> keywords,
            Slot slot
    ) :
            id(id),
            name(std::move(name)),
            shortDescription(std::move(shortDescription)),
            longDescription(std::move(longDescription)),
            keywords(std::move(keywords)),
            slot(slot),
            extraObjectInfo({}) {}

    Object::Object(
            model::ID id,
            std::string name,
            std::string shortDescription,
            std::vector<std::string> longDescription,
            std::vector<std::string> keywords,
            Slot slot,
            ExtraObjectInfo extraObjectInfo
    ) :
            id(id),
            name(std::move(name)),
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

    std::string Object::getName() const {
        return this->name;
    }

    void Object::setName(std::string name) {
        this->name = std::move(name);
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

    ExtraObjectInfo Object::getExtraObjectInfo() const {
        return extraObjectInfo;
    }

    void Object::setExtraObjectInfo(ExtraObjectInfo extraObjectInfo) {
        Object::extraObjectInfo = std::move(extraObjectInfo);
    }

    bool Object::canBeEquipped() const {
        return this->slot >= 0 && this->slot < Slot::Misc;
    }

    bool Object::operator==(const Object &NPC) const {
        return this->id == NPC.id;
    }
}