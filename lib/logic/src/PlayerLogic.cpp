//
// Created by jnhkm on 2019-02-04.
//

#include "PlayerLogic.h"

using logic::PlayerLogic;

namespace logic {
    PlayerLogic::PlayerLogic() {}

    bool PlayerLogic::canEquipItem(const std::map<model::ID, Object> &items, const Object &item) {
        return item.canBeEquipped() && isItemInInventory(items, item);
    }

    bool PlayerLogic::isItemInInventory(const std::map<model::ID, Object> &items, const Object &item) {
        return static_cast<bool>(items.count(item.getId()));
    }

    bool PlayerLogic::isItemEquipped(const std::map<int, Object> &items, const Object &item) {
        return  isSlotOccupied(items, item.getSlot()) && items.at(item.getSlot()).getId() == item.getId();
    }

    bool PlayerLogic::isSlotOccupied(const std::map<int, Object> &items, const Slot &slot) {
        return static_cast<bool>(items.count(slot));
    }
}
