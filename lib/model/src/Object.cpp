//
// Created by Waswa Olunga on 2019-01-18.
//

#include <Object.h>

namespace model {

    Object::Object() :
        id(Object::DEFAULT_ID),
        name({}),
        description({}),
        canEquip(DEFAULT_CAN_EQUIP),
        slot(Slot::Misc){}


    Object::Object(int id, std::string name, std::string description, Slot slot) :
        id(id),
        name(std::move(name)),
        description(std::move(description)),
        canEquip(DEFAULT_CAN_EQUIP),
        slot(slot) {}

    Object::Object(int id, std::string name, std::string description, bool canEquip, Slot slot) :
        id(id),
        name(std::move(name)),
        description(std::move(description)),
        canEquip(canEquip),
        slot(slot) {}

    int Object::getId() const {
        return this->id;
    }

    void Object::setId(int id){
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


    Slot Object::getSlot() const {
        return this->slot;
    }

    void Object::setSlot(model::Slot slot) {
        this->slot = slot;
    }

    bool Object::canBeEquipped() const {
        return this->canEquip;
    }

    void Object::setCanEquip(bool canEquip) {
        this->canEquip = canEquip;
    }

    bool Object::operator==(const Object& NPC) const {
        return this->id == NPC.id;
    }
}