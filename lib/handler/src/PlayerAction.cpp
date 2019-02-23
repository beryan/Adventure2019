//
// Created by jnhkm on 2019-02-04.
//

#include "PlayerAction.h"

using action::PlayerAction;

namespace action {
    PlayerAction::PlayerAction() {}

    void PlayerAction::equipItem(Player &player, Object item) {
        if (logic.canEquipItem(player.getInventory().getMappedInventory(), item)) {
            if (logic.isSlotOccupied(player.getMappedEquippedItems(), item.getSlot())) {
                unequipSlot(player, item.getSlot());
            }

            player.addToEquippedItems(item);
            player.getInventory().removeItemFromInventory(item);
        }
    }

    void PlayerAction::unequipSlot(Player &player, const Slot &slot) {
        if (logic.isSlotOccupied(player.getMappedEquippedItems(), slot)) {
            player.getInventory().addItemToInventory(player.getMappedEquippedItems().at(slot));

            player.removeEquippedItem(slot);
        }
    }

    void PlayerAction::addToInventoryItems(Player &player, Object item) {
        player.getInventory().addItemToInventory(item);
    }

    Object PlayerAction::dropItemFromInventory(Player &player, Object item) {
        auto temp_item = player.getInventory().removeItemFromInventory(item);

        return std::move(temp_item);
    }

    Object PlayerAction::dropItemFromEquipped(Player &player, Slot slot) {
        auto temp_item = player.removeEquippedItem(slot);

        return std::move(temp_item);
    }
}
