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
    public:
        PlayerLogic();

        bool canEquipItem(Inventory items, const Object &item) const;

        bool isItemEquipped(Equipment items, const Object &item) const;
    };

}

#endif //PLAYERLOGIC_H
