//
// Created by Waswa Olunga on 2019-01-18.
//

#include <Object.h>

namespace model {

    Object::Object() :
        id(Object::DEFAULT_ID),
        name({}),
        description({}),
        slot(Slot::Misc){}

    Object::Object(model::ID id, std::string name, std::string description, Slot slot) :
        id(id),
        name(std::move(name)),
        description(std::move(description)),
        slot(slot){}

    model::ID Object::getId() const {
        return this->id;
    }

    void Object::setId(model::ID id){
        this->id = id;
    }

    std::string Object::getName() const {
        return this->name;
    }

    void Object::setName(std::string name){
        this->name = std::move(name);
    }

    std::string Object::getDescription() const {
        return this->description;
    }

    void Object::setDescription(std::string description) {
        this->description = std::move(description);
    }

    std::string Object::getShortDescription() const {
        return this->shortDescription;
    }

    void Object::setShortDescription(std::string shortDescription) {
        this->shortDescription = std::move(shortDescription);
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

    bool Object::canBeEquipped() const {
        return this->slot >= 0 && this->slot < Slot::Misc;
    }

    bool Object::operator==(const Object& NPC) const {
        return this->id == NPC.id;
    }
}