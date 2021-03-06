//
// Created by jnhkm on 2019-02-04.
//

#include "PlayerLogic.h"

using logic::PlayerLogic;

namespace logic {
    PlayerLogic::PlayerLogic() {}

    bool PlayerLogic::canEquipItem(Inventory items, const Object &item) const {
        return item.canBeEquipped() && items.isItemInInventory(item);
    }
}
