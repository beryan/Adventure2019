//
// Created by jnhkm on 2019-02-04.
//

#include "PlayerAction.h"

using action::PlayerAction;

namespace action {
    PlayerAction::PlayerAction() {}

    void PlayerAction::equipItem(Player &player, Object item) {
        if (logic.canEquipItem(player.getInventory(), item)) {
            if (player.getEquipments().isSlotOccupied(item.getSlot())) {
                player.getEquipments().unequipSlot(item.getSlot());
            }

            player.getEquipments().equipItem(player.getInventory().removeItemFromInventory(item));
        }
    }

    void PlayerAction::unequipItem(Player &player, Object &item) {
        if (player.getEquipments().isItemEquipped(item)) {
            player.getInventory().addItemToInventory(player.getEquipments().unequipItem(item));
        }
    }

    void PlayerAction::pickupItem(Player &player, Object item) {
        player.getInventory().addItemToInventory(item);
    }

    Object PlayerAction::dropItem(Player &player, Object item) {
        Object temp_item;

        if (player.getInventory().isItemInInventory(item)) {
            temp_item = player.getInventory().removeItemFromInventory(item);
        }
        else if (player.getEquipments().isItemEquipped(item)) {
            temp_item = player.getEquipments().unequipItem(item);
        }

        return std::move(temp_item);
    }
}
