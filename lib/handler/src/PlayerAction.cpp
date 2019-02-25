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
                unequipSlot(player, item.getSlot());
            }

            player.getEquipments().equipItem(player.getInventory().removeItemFromInventory(item));
        }
    }

    void PlayerAction::unequipSlot(Player &player, const Slot &slot) {
        if (player.getEquipments().isSlotOccupied(slot)) {
            player.getInventory().addItemToInventory(player.getEquipments().getMappedEquipments().at(slot));

            player.getEquipments().unequipSlot(slot);
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
