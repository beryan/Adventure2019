//
// Created by Waswa Olunga on 2019-01-18.
//

#ifndef WEBSOCKETNETWORKING_ITEM_H
#define WEBSOCKETNETWORKING_ITEM_H

#include <string>
#include "Object.h"

namespace model {

/**
 *  @class Item
 *
 *  @brief A class describing an item.
 *
 *  The Item class contains all the properties of
 *  an item. These properties can be
 *  accessed and changed.
 */

    enum class Slot {Head, Body, Legs, Arm, Weapon};

    class Item {
    public:
        Item(int id, std::string name, std::string description, bool canEquip, Slot slot)
            : object(id, std::move(name), std::move(description)),
              canEquip(canEquip),
              slot(slot) {}

        Slot getSlot();

        void setSlot(Slot slot);

        bool getCanEquip();

        void setCanEquip(bool canEquip);

    private:
        Object object;

        bool canEquip;

        Slot slot;
    };
}

#endif //WEBSOCKETNETWORKING_ITEM_H