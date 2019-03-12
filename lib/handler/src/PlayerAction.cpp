//
// Created by jnhkm on 2019-02-04.
//

#include "PlayerAction.h"

using action::PlayerAction;

namespace action {
    PlayerAction::PlayerAction() {}

    bool PlayerAction::equipItem(Player &player, const Object &item) {
        bool success = false;
        if (logic.canEquipItem(player.getInventory(), item)) {
            if (player.getEquipment().isSlotOccupied(item.getSlot())) {
                auto prevItem = player.getEquipment().unequipSlot(item.getSlot());
                player.getInventory().addItemToInventory(prevItem);
            }
            player.getEquipment().equipItem(player.getInventory().removeItemFromInventory(item));
            success = true;
        }
        return success;
    }

    void PlayerAction::unequipItem(Player &player, const Object &item) {
        if (player.getEquipment().isItemEquipped(item)) {
            player.getInventory().addItemToInventory(player.getEquipment().unequipItem(item));
        }
    }

    void PlayerAction::pickupItem(Player &player, const Object &item) {
        player.getInventory().addItemToInventory(item);
    }

    Object PlayerAction::dropItem(Player &player, const Object &item) {
        Object temp_item;

        if (player.getInventory().isItemInInventory(item)) {
            temp_item = player.getInventory().removeItemFromInventory(item);
        }
        else if (player.getEquipment().isItemEquipped(item)) {
            temp_item = player.getEquipment().unequipItem(item);
        }

        return std::move(temp_item);
    }
}
