//
// Created by jnhkm on 2019-02-04.
//

#include "PlayerAction.h"
#include "GameLogic.h"

using action::PlayerAction;
using logic::PlayerLogic;

namespace action {
    void PlayerAction::equipItem(Player &player, Object item) {
        if (PlayerLogic::canEquipItem(player.getMappedInventoryItems(), item)) {
            if (PlayerLogic::isSlotOccupied(player.getMappedEquippedItems(), item.getSlot()))
                unequipSlot(player, item.getSlot());

            player.addToEquippedItems(item);
            player.removeInventoryItem(item);
        }
    }

    void PlayerAction::unequipSlot(Player &player, const Slot &slot) {
        if (PlayerLogic::isSlotOccupied(player.getMappedEquippedItems(), slot)) {
            player.addToInventoryItems(player.getMappedEquippedItems().at(slot));

            player.removeEquippedItem(slot);
        }
    }

    void PlayerAction::addToInventoryItems(Player &player, Object item) {
        player.addToInventoryItems(item);
    }

    Object PlayerAction::dropItemFromInventory(Player &player, Object item) {
        auto temp_item = player.removeInventoryItem(item);

        return std::move(temp_item);
    }

    Object PlayerAction::dropItemFromEquipped(Player &player, Slot slot) {
        auto temp_item = player.removeEquippedItem(slot);

        return std::move(temp_item);
    }
}
