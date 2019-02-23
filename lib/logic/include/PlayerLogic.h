//
// Created by jnhkm on 2019-02-04.
//

#ifndef PLAYERLOGIC_H
#define PLAYERLOGIC_H

#include "World.h"

using model::Slot;
using model::Object;

namespace logic {
    class PlayerLogic {
    private:
        PlayerLogic();
    public:
        static bool canEquipItem(const std::map<model::ID, Object> &items, const Object &item);

        static bool isItemInInventory(const std::map<model::ID, Object> &items, const Object &item);

        static bool isItemEquipped(const std::map<int, Object> &items, const Object &item);

        static bool isSlotOccupied(const std::map<int, Object> &items, const Slot &slot);
    };

}

#endif //PLAYERLOGIC_H
