//
// Created by Waswa Olunga on 2019-01-18.
//

#ifndef WEBSOCKETNETWORKING_ITEM_H
#define WEBSOCKETNETWORKING_ITEM_H

#include <string>
#include "Object.h"

const bool DEFAULT_CAN_EQUIP = false;

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

    // Count returns the length of the Slots
    enum Slot {Head, Body, Legs, Arm, Weapon, Misc, Count};

    class Item {
    public:
        Item();

        Item(int id, std::string name, std::string description, Slot slot);

        Item(int id, std::string name, std::string description, bool canEquip, Slot slot);

        Slot getSlot();

        void setSlot(Slot slot);

        bool getCanEquip();

        void setCanEquip(bool canEquip);

        Object getObject();

        void setObject(Object object);

        bool operator==(const Item& rhs) const;

    private:
        Object object;

        bool canEquip;

        Slot slot;
    };
}

#endif //WEBSOCKETNETWORKING_ITEM_H