//
// Created by jnhkm on 2019-02-04.
//

#include "PlayerAction.h"
#include "GameLogic.h"

using action::PlayerAction;
using logic::PlayerLogic;

namespace action {
    void PlayerAction::equipItem(Player &player, Object item) {
        std::map<model::ID, Object>& inventory = player.getMappedInventoryItems();

        if (PlayerLogic::canEquipItem(inventory, item)) {
            std::map<int, Object>& equipments = player.getMappedEquippedItems();

            if (PlayerLogic::isSlotOccupied(equipments, item.getSlot()))
                unequipSlot(player, item.getSlot());

            equipments.insert(std::pair<int, Object>(item.getSlot(), std::move(item)));
            inventory.erase(item.getId());
        }
    }

    void PlayerAction::unequipSlot(Player &player, const Slot &slot) {
        std::map<int, Object>& equipments = player.getMappedEquippedItems();

        if (PlayerLogic::isSlotOccupied(equipments, slot)) {
            addToInventoryItems(player, equipments.at(slot));

            equipments.erase(slot);
        }
    }

    void PlayerAction::addToInventoryItems(Player &player, Object item) {
        player.getMappedInventoryItems().insert(std::pair<model::ID, Object>(item.getId(), std::move(item)));
    }

    Object PlayerAction::dropItemFromInventory(Player &player, Object item) {
        std::map<model::ID, Object>& inventory = player.getMappedInventoryItems();

        auto temp_item = inventory.at(item.getId());
        inventory.erase(item.getId());

        return std::move(temp_item);
    }

    Object PlayerAction::dropItemFromEquipped(Player &player, Slot slot) {
        std::map<int, Object>& equipments = player.getMappedEquippedItems();

        auto temp_item = equipments.at(slot);
        equipments.erase(slot);

        return std::move(temp_item);
    }
}
