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
        std::map<int, Object> equipment;
    public:
        Equipment();

        std::map<int, Object> getMappedEquipment();

        std::vector<Object> getVectorEquipment() const;

        void mapEquipment(std::vector<Object> &items);

        void equipItem(Object object);

        Object unequipItem(Object &item);

        Object unequipSlot(Slot slot);

        bool isSlotOccupied(const Slot &slot);

        bool isItemEquipped(const Object &item);
    };
}



#endif //EQUIPMENT_H
