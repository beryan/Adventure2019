//
// Created by jnhkm on 2019-01-25.
//

#include "lib/gtest/gtest.h"
#include "lib/gmock/gmock.h"
#include "PlayerAction.h"
#include "PlayerLogic.h"
#include <stdlib.h>
#include <iostream>

using action::PlayerAction;
using logic::PlayerLogic;

namespace {
    TEST(PlayerTestSuite, canConstructPlayer) {
        int expected_id = 12345;
        std::string expected_username = "hello";
        std::string expected_password = "zombie";

        Player player{expected_id, expected_username, expected_password};

        EXPECT_EQ(player.getId(), expected_id);
        EXPECT_EQ(player.getUsername(), expected_username);
        EXPECT_EQ(player.getPassword(), expected_password);
    }

    TEST(PlayerTestSuite, canAddItemToInventory) {
        Player player{152, "hello", "20000"};
        Object item{12345, "Booboo", "janga", {}, {}, Slot::Head};

        PlayerAction::addToInventoryItems(player, item);

        EXPECT_TRUE(PlayerLogic::isItemInInventory(player.getMappedInventoryItems(), item));
    }

    TEST(PlayerTestSuite, canDropItemFromInventory) {
        Player player{152, "hello", "20000"};
        Object item{12345, "Booboo", "janga", {}, {}, Slot::Head};

        PlayerAction::addToInventoryItems(player, item);
        PlayerAction::dropItemFromInventory(player, item);

        EXPECT_FALSE(PlayerLogic::isItemInInventory(player.getMappedInventoryItems(), item));
    }

    TEST(PlayerTestSuite, canEquipItemFromInventoryWhenSlotIsEmpty) {
        Player player{152, "hello", "20000"};
        Object item{12345, "Booboo", "janga", {}, {}, Slot::Head};

        PlayerAction::addToInventoryItems(player, item);
        PlayerAction::equipItem(player, item);

        EXPECT_TRUE(PlayerLogic::isItemEquipped(player.getMappedEquippedItems(), item));
    }

    TEST(PlayerTestSuite, canEquipItemFromInventoryWhenSlotIsOccupied) {
        Player player{152, "hello", "20000"};
        Object item{12345, "Booboo", "janga", {}, {}, Slot::Head};

        PlayerAction::addToInventoryItems(player, item);
        PlayerAction::equipItem(player, item);

        Object item2{121, "Hello", "world", {}, {}, Slot::Head};

        PlayerAction::addToInventoryItems(player, item2);
        PlayerAction::equipItem(player, item2);

        EXPECT_TRUE(PlayerLogic::isItemInInventory(player.getMappedInventoryItems(), item));
        EXPECT_TRUE(PlayerLogic::isItemEquipped(player.getMappedEquippedItems(), item2));
    }

    TEST(PlayerTestSuite, cannotEquipFromOutsideOfInventory) {
        Player player{152, "hello", "20000"};
        Object item{12345, "Booboo", "janga", {}, {}, Slot::Head};

        PlayerAction::equipItem(player, item);

        EXPECT_FALSE(PlayerLogic::isItemEquipped(player.getMappedEquippedItems(), item));
    }

    TEST(PlayerTestSuite, canDropEquippedItem) {
        Player player{152, "hello", "20000"};
        Object item{12345, "Booboo", "janga", {}, {}, Slot::Head};

        PlayerAction::addToInventoryItems(player, item);
        PlayerAction::equipItem(player, item);

        Object droppedItem = PlayerAction::dropItemFromEquipped(player, item.getSlot());

        EXPECT_FALSE(PlayerLogic::isItemEquipped(player.getMappedEquippedItems(), item));
        EXPECT_EQ(droppedItem.getId(), item.getId());
    }

    TEST(PlayerTestSuite, canUnequipItem) {
        Player player{152, "hello", "20000"};
        Object item{12345, "Booboo", "janga", {}, {}, Slot::Head};

        PlayerAction::addToInventoryItems(player, item);
        PlayerAction::equipItem(player, item);

        PlayerAction::unequipSlot(player, Slot::Head);

        EXPECT_FALSE(PlayerLogic::isItemEquipped(player.getMappedEquippedItems(), item));
        EXPECT_TRUE(PlayerLogic::isItemInInventory(player.getMappedInventoryItems(), item));
    }

    TEST(PlayerTestSuite, canReturnCollectionOfItems) {
        Player player{1294, "Cindy", "iPoopOnPizzas"};

        unsigned int itemsToCreate = 10;
        for (unsigned int i = 0; i < itemsToCreate; i++) {
            Object item{rand() % 220, "test", "test", {}, {}, Slot::Head};
            PlayerAction::addToInventoryItems(player, item);
        }

        std::vector<Object> items = player.getInventoryItems();

        EXPECT_EQ(items.size(), itemsToCreate);
    }

    TEST(PlayerTestSuite, canSetCurrentRoomIDwithConstructor) {
        model::ID expected_roomID = 41;
        Player player{1234, "poopy", "mcpoop", expected_roomID};

        EXPECT_EQ(player.getCurrRoomID(), expected_roomID);
    }

    TEST(PlayerTestSuite, canSetCurrentRoomIDwithSetter) {
        model::ID expected_roomID = 41;
        Player player{1234, "poopy", "mcpoop"};
        player.setCurrRoomID(expected_roomID);

        EXPECT_EQ(player.getCurrRoomID(), expected_roomID);
    }
}