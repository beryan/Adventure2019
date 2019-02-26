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
    public:
        Equipment();

        std::unordered_map<int, Object> getMappedEquipment();

        std::vector<Object> getVectorEquipment() const;

        void mapEquipment(std::vector<Object> items);

        void equipItem(const Object &object);

        Object unequipItem(const Object &item);

        Object unequipSlot(const Slot &slot);

        bool isSlotOccupied(const Slot &slot);

        bool isItemEquipped(const Object &item);
    };
}



#endif //EQUIPMENT_H
