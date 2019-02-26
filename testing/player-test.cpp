//
// Created by jnhkm on 2019-01-25.
//

#include "lib/gtest/gtest.h"
#include "lib/gmock/gmock.h"
#include "PlayerAction.h"
#include <stdlib.h>
#include <iostream>

using action::PlayerAction;

namespace {
    class PlayerTestSuite : public ::testing::Test {
    protected:
        Player player;
        PlayerAction action;

        virtual void SetUp() override {
            int expected_id = 12345;
            std::string expected_username = "hello";
            std::string expected_password = "world";
            model::ID expected_roomID = 41;

            player = {expected_id, expected_username, expected_password, expected_roomID};
            action = {};
        }
    };

    TEST_F(PlayerTestSuite, canAddItemToInventory) {
        Object item{12345, "Booboo", "janga", {}, {}, Slot::Head};

        action.pickupItem(player, item);

        EXPECT_TRUE(player.getInventory().isItemInInventory(item));
    }

    TEST_F(PlayerTestSuite, canDropItemAndPickItUp) {
        Object item{12345, "Booboo", "janga", {}, {}, Slot::Head};

        action.pickupItem(player, item);
        action.dropItem(player, item);

        ASSERT_FALSE(player.getInventory().isItemInInventory(item));

        action.pickupItem(player, item);

        EXPECT_TRUE(player.getInventory().isItemInInventory(item));
    }

    TEST_F(PlayerTestSuite, canDropItemFromInventory) {
        Object item{12345, "Booboo", "janga", {}, {}, Slot::Head};

        action.pickupItem(player, item);
        action.dropItem(player, item);

        EXPECT_FALSE(player.getInventory().isItemInInventory(item));
    }

    TEST_F(PlayerTestSuite, canPickupItemFromRoomInventory) {
        Inventory inventory{};
        Object item{12345, "Booboo", "janga", {}, {}, Slot::Body};
        unsigned int expected_size = 0;

        inventory.addItemToInventory(item);

        ASSERT_TRUE(inventory.isItemInInventory(item));

        action.pickupItem(player, inventory.removeItemFromInventory(item));

        ASSERT_FALSE(inventory.isItemInInventory(item));
        EXPECT_EQ(expected_size, inventory.getVectorInventory().size());
        EXPECT_TRUE(player.getInventory().isItemInInventory(item));
    }

    TEST_F(PlayerTestSuite, canEquipTwoDifferentSlotItemsFromInventory) {
        Object item{12345, "Booboo", "janga", {}, {}, Slot::Head};
        Object item2{22345, "Zooboo", "Danga", {}, {}, Slot::Body};

        action.pickupItem(player, item);
        action.pickupItem(player, item2);

        action.equipItem(player, item);
        action.equipItem(player, item2);

        EXPECT_TRUE(player.getEquipment().isItemEquipped(item));
        EXPECT_TRUE(player.getEquipment().isSlotOccupied(item.getSlot()));

        EXPECT_TRUE(player.getEquipment().isItemEquipped(item2));
        EXPECT_TRUE(player.getEquipment().isSlotOccupied(item2.getSlot()));
    }

    TEST_F(PlayerTestSuite, canEquipItemFromInventoryWhenSlotIsEmpty) {
        Object item{12345, "Booboo", "janga", {}, {}, Slot::Head};

        action.pickupItem(player, item);
        action.equipItem(player, item);

        EXPECT_TRUE(player.getEquipment().isItemEquipped(item));
        EXPECT_TRUE(player.getEquipment().isSlotOccupied(item.getSlot()));
    }

    TEST_F(PlayerTestSuite, canEquipItemFromInventoryWhenSlotIsOccupied) {
        Object item{12345, "Booboo", "janga", {}, {}, Slot::Head};

        action.pickupItem(player, item);
        action.equipItem(player, item);

        Object item2{121, "Hello", "world", {}, {}, Slot::Head};

        action.pickupItem(player, item2);
        action.equipItem(player, item2);

        EXPECT_TRUE(player.getEquipment().isItemEquipped(item2));
        EXPECT_TRUE(player.getEquipment().isSlotOccupied(item2.getSlot()));
    }

    TEST_F(PlayerTestSuite, cannotEquipFromOutsideOfInventory) {
        Object item{12345, "Booboo", "janga", {}, {}, Slot::Head};

        action.equipItem(player, item);

        EXPECT_FALSE(player.getEquipment().isItemEquipped(item));
        EXPECT_FALSE(player.getEquipment().isSlotOccupied(item.getSlot()));
    }

    TEST_F(PlayerTestSuite, canDropEquippedItem) {
        Object item{12345, "Booboo", "janga", {}, {}, Slot::Head};

        action.pickupItem(player, item);
        action.equipItem(player, item);

        Object droppedItem = action.dropItem(player, item);

        EXPECT_FALSE(player.getEquipment().isItemEquipped(item));
        EXPECT_FALSE(player.getEquipment().isSlotOccupied(item.getSlot()));
        EXPECT_TRUE(droppedItem == item);
    }

    TEST_F(PlayerTestSuite, canUnequipItem) {
        Object item{12345, "Booboo", "janga", {}, {}, Slot::Head};

        action.pickupItem(player, item);
        action.equipItem(player, item);

        action.unequipItem(player, item);

        EXPECT_FALSE(player.getEquipment().isItemEquipped(item));
        EXPECT_FALSE(player.getEquipment().isSlotOccupied(item.getSlot()));
        EXPECT_TRUE(player.getInventory().isItemInInventory(item));
    }

    TEST_F(PlayerTestSuite, canReturnCollectionOfItems) {
        unsigned int itemsToCreate = 10;
        for (unsigned int i = 0; i < itemsToCreate; i++) {
            Object item{rand() % 220, "test", "test", {}, {}, Slot::Head};
            action.pickupItem(player, item);
        }

        std::vector<Object> items = player.getInventory().getVectorInventory();

        ASSERT_EQ(items.size(), itemsToCreate);
        for (Object object : items) {
            EXPECT_TRUE(object.getSlot() == Slot::Head);
        }
    }

    TEST_F(PlayerTestSuite, canSetCurrentRoomIDwithConstructor) {
        EXPECT_EQ(41, player.getCurrRoomID());
    }

    TEST_F(PlayerTestSuite, canSetCurrentRoomIDwithSetter) {
        model::ID expected_roomID = 61;

        player.setCurrRoomID(expected_roomID);

        EXPECT_EQ(expected_roomID, player.getCurrRoomID());
    }
}