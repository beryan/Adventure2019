//
// Created by jnhkm on 2019-02-04.
//

#include "PlayerHandler.h"

using handler::PlayerHandler;

namespace handler {
    PlayerHandler::PlayerHandler() {}

    bool PlayerHandler::equipItem(Player &player, const Object &item) {
        bool success = false;
        if (logic.canEquipItem(player.getMutableInventory(), item)) {
            if (player.getMutableEquipment().isSlotOccupied(item.getSlot())) {
                auto prevItem = player.getMutableEquipment().unequipSlot(item.getSlot());
                player.getMutableInventory().addItemToInventory(prevItem);
            }
            player.getMutableEquipment().equipItem(player.getMutableInventory().removeItemFromInventory(item));
            success = true;
        }
        return success;
    }

    void PlayerHandler::unequipItem(Player &player, const Object &item) {
        if (player.getMutableEquipment().isItemEquipped(item)) {
            player.getMutableInventory().addItemToInventory(player.getMutableEquipment().unequipItem(item));
        }
    }

    void PlayerHandler::pickupItem(Player &player, const Object &item) {
        player.getMutableInventory().addItemToInventory(item);
    }

    void PlayerHandler::giveItem(Player &giver, Player &receiver, const Object &item) {
        if (giver.getMutableInventory().isItemInInventory(item) || giver.getMutableEquipment().isItemEquipped(item)) {
            receiver.getMutableInventory().addItemToInventory(dropItem(giver, item));
        }
    }

    Object PlayerHandler::dropItem(Player &player, const Object &item) {
        Object temp_item;

        if (player.getMutableInventory().isItemInInventory(item)) {
            temp_item = player.getMutableInventory().removeItemFromInventory(item);
        }
        else if (player.getMutableEquipment().isItemEquipped(item)) {
            temp_item = player.getMutableEquipment().unequipItem(item);
        }

        return temp_item;
    }
}
