//
// Created by Waswa Olunga on 2019-01-18.
//

#include <Item.h>

using model::Item;
using model::Slot;

namespace model {

    Item::
    Item()
        : object(-1 , "", ""),
          canEquip(DEFAULT_CAN_EQUIP),
          slot(Slot::Misc){}


    Item::
    Item(int id, std::string name, std::string description, Slot slot)
        : object(id, std::move(name), std::move(description)),
          canEquip(DEFAULT_CAN_EQUIP),
          slot(slot) {}

    Item::
    Item(int id, std::string name, std::string description, bool canEquip, Slot slot)
        : object(id, std::move(name), std::move(description)),
          canEquip(canEquip),
          slot(slot) {}

    Slot
    Item::getSlot() {
        return this->slot;
    }

    void
    Item::setSlot(model::Slot slot) {
        this->slot = slot;
    }

    bool
    Item::getCanEquip() {
        return this->canEquip;
    }

    void
    Item::setCanEquip(bool canEquip) {
        this->canEquip = canEquip;
    }

    Object
    Item::getObject() {
        return object;
    }

    void
    Item::setObject(Object object) {
        this->object = std::move(object);
    }

    bool Item::operator==(const Item& rhs) const {
        return object == rhs.object;
    }
}