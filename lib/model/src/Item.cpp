//
// Created by Waswa Olunga on 2019-01-18.
//

#include <Item.h>

using model::Item;

namespace model {

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
}