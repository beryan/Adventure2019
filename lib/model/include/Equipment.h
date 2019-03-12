//
// Created by jnhkm on 2019-02-23.
//

#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include "Object.h"

using model::Object;

namespace model {
    class Equipment {
    private:
        /**
         * int: Numbers indicate which slot the item is in
         * Object: Item that is equipped
         */
        std::unordered_map<int, Object> equipment;

        friend std::ostream&operator<<(std::ostream& os, const Equipment& inv);
    public:
        Equipment();

        /**
         * @return the immutable unordered map of Equipment
         */
        std::unordered_map<int, Object> getMappedEquipment();

        /**
         * @return the immutable vector form of Equipment
         */
        std::vector<Object> getVectorEquipment() const;

        /**
         * Maps the list of items into Equipment
         * @param items: Items to be mapped
         */
        void mapEquipment(const std::vector<Object> &items);

        /**
         * Equips an item
         * @param object: Item to be equipped
         */
        void equipItem(const Object &object);

        /**
         * Uneqiups an item
         * @param item: Item to be unequipped
         * @return Ownership of the item that was unequipped
         */
        Object unequipItem(const Object &item);

        /**
         * Unequips a specific slot in Equipment
         * @param slot: Slot to be unequipped
         * @return Returns the ownership of the item that existed in the slot that was
         * unequipped
         */
        Object unequipSlot(const Slot &slot);

        /**
         * Checks to see if the Slot is currently occupied by an item
         * @param slot: Slot to be checked
         * @return True if an item exists in the slot, False if not
         */
        bool isSlotOccupied(const Slot &slot);

        /**
         * Checks to see if a specific item is equipped
         * @param item: Item to be checked
         * @return Returns True if the item is currently equipped, False if not
         */
        bool isItemEquipped(const Object &item);
    };
}



#endif //EQUIPMENT_H
