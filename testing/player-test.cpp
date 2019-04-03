//
// Created by jnhkm on 2019-01-25.
//

#include "lib/gtest/gtest.h"
#include "lib/gmock/gmock.h"
#include "PlayerHandler.h"
#include <stdlib.h>
#include <iostream>

using handler::PlayerHandler;

namespace {
    class PlayerTestSuite : public ::testing::Test {
    protected:
        Player player;
        PlayerHandler handler;

        virtual void SetUp() override {
            int expected_id = 12345;
            std::string expected_username = "hello";
            std::string expected_password = "world";
            model::ID expected_roomID = 41;

            player = {expected_id, expected_username, expected_password, expected_roomID};
            handler = {};
        }
    };

    TEST_F(PlayerTestSuite, canAddItemToInventory) {
        Object item{12345, "janga", {}, {}, Slot::Head};

        handler.pickupItem(player, item);

        EXPECT_TRUE(player.getMutableInventory().isItemInInventory(item));
    }

    TEST_F(PlayerTestSuite, canDropItemAndPickItUp) {
        Object item{12345, "janga", {}, {}, Slot::Head};

        handler.pickupItem(player, item);
        handler.dropItem(player, item);

        ASSERT_FALSE(player.getMutableInventory().isItemInInventory(item));

        handler.pickupItem(player, item);

        EXPECT_TRUE(player.getMutableInventory().isItemInInventory(item));
    }

    TEST_F(PlayerTestSuite, canDropItemFromInventory) {
        Object item{12345, "janga", {}, {}, Slot::Head};

        handler.pickupItem(player, item);
        handler.dropItem(player, item);

        EXPECT_FALSE(player.getMutableInventory().isItemInInventory(item));
    }

    TEST_F(PlayerTestSuite, canPickupItemFromRoomInventory) {
        Inventory inventory{};
        Object item{12345, "janga", {}, {}, Slot::Chest};
        unsigned int expected_size = 0;

        inventory.addItemToInventory(item);

        ASSERT_TRUE(inventory.isItemInInventory(item));

        handler.pickupItem(player, inventory.removeItemFromInventory(item));

        ASSERT_FALSE(inventory.isItemInInventory(item));
        EXPECT_EQ(expected_size, inventory.getVectorInventory().size());
        EXPECT_TRUE(player.getMutableInventory().isItemInInventory(item));
    }

    TEST_F(PlayerTestSuite, canEquipTwoDifferentSlotItemsFromInventory) {
        Object item{12345, "janga", {}, {}, Slot::Head};
        Object item2{22345, "Danga", {}, {}, Slot::Chest};

        handler.pickupItem(player, item);
        handler.pickupItem(player, item2);

        handler.equipItem(player, item);
        handler.equipItem(player, item2);

        EXPECT_TRUE(player.getMutableEquipment().isItemEquipped(item));
        EXPECT_TRUE(player.getMutableEquipment().isSlotOccupied(item.getSlot()));

        EXPECT_TRUE(player.getMutableEquipment().isItemEquipped(item2));
        EXPECT_TRUE(player.getMutableEquipment().isSlotOccupied(item2.getSlot()));
    }

    TEST_F(PlayerTestSuite, canEquipItemFromInventoryWhenSlotIsEmpty) {
        Object item{12345, "janga", {}, {}, Slot::Head};

        handler.pickupItem(player, item);
        handler.equipItem(player, item);

        EXPECT_TRUE(player.getMutableEquipment().isItemEquipped(item));
        EXPECT_TRUE(player.getMutableEquipment().isSlotOccupied(item.getSlot()));
    }

    TEST_F(PlayerTestSuite, canEquipItemFromInventoryWhenSlotIsOccupied) {
        Object item{12345, "janga", {}, {}, Slot::Head};

        handler.pickupItem(player, item);
        handler.equipItem(player, item);

        Object item2{121, "world", {}, {}, Slot::Head};

        handler.pickupItem(player, item2);
        handler.equipItem(player, item2);

        EXPECT_TRUE(player.getMutableEquipment().isItemEquipped(item2));
        EXPECT_TRUE(player.getMutableEquipment().isSlotOccupied(item2.getSlot()));
    }

    TEST_F(PlayerTestSuite, cannotEquipFromOutsideOfInventory) {
        Object item{12345, "janga", {}, {}, Slot::Head};

        handler.equipItem(player, item);

        EXPECT_FALSE(player.getMutableEquipment().isItemEquipped(item));
        EXPECT_FALSE(player.getMutableEquipment().isSlotOccupied(item.getSlot()));
    }

    TEST_F(PlayerTestSuite, canDropEquippedItem) {
        Object item{12345, "janga", {}, {}, Slot::Head};

        handler.pickupItem(player, item);
        handler.equipItem(player, item);

        Object droppedItem = handler.dropItem(player, item);

        EXPECT_FALSE(player.getMutableEquipment().isItemEquipped(item));
        EXPECT_FALSE(player.getMutableEquipment().isSlotOccupied(item.getSlot()));
        EXPECT_TRUE(droppedItem == item);
    }

    TEST_F(PlayerTestSuite, canUnequipItem) {
        Object item{12345, "janga", {}, {}, Slot::Head};

        handler.pickupItem(player, item);
        handler.equipItem(player, item);

        handler.unequipItem(player, item);

        EXPECT_FALSE(player.getMutableEquipment().isItemEquipped(item));
        EXPECT_FALSE(player.getMutableEquipment().isSlotOccupied(item.getSlot()));
        EXPECT_TRUE(player.getMutableInventory().isItemInInventory(item));
    }

    TEST_F(PlayerTestSuite, canReturnCollectionOfItems) {
        unsigned int itemsToCreate = 10;
        for (unsigned int i = 0; i < itemsToCreate; i++) {
            Object item{rand() % 220, "test", {}, {}, Slot::Head};
            handler.pickupItem(player, item);
        }

        std::vector<Object> items = player.getMutableInventory().getVectorInventory();

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

    TEST_F(PlayerTestSuite, canNotGiveItemToDifferentPlayerWhenObjectIsNotInInventory) {
        Player receiver{1241525, "Mo", "Do"};

        Object item{12345, "janga", {}, {}, Slot::Head};

        ASSERT_FALSE(player.getMutableInventory().isItemInInventory(item));

        handler.giveItem(player, receiver, item);

        ASSERT_FALSE(player.getMutableInventory().isItemInInventory(item));
        ASSERT_FALSE(player.getMutableEquipment().isItemEquipped(item));
        EXPECT_FALSE(receiver.getMutableInventory().isItemInInventory(item));
    }

    TEST_F(PlayerTestSuite, canGiveItemToDifferentPlayerWhenObjectInInventory) {
        Player receiver{1241525, "Mo", "Do"};

        Object item{12345, "janga", {}, {}, Slot::Head};
        handler.pickupItem(player, item);

        ASSERT_TRUE(player.getMutableInventory().isItemInInventory(item));

        handler.giveItem(player, receiver, item);

        ASSERT_FALSE(player.getMutableInventory().isItemInInventory(item));
        ASSERT_FALSE(player.getMutableEquipment().isItemEquipped(item));
        EXPECT_TRUE(receiver.getMutableInventory().isItemInInventory(item));
    }

    TEST_F(PlayerTestSuite, canGiveItemToDifferentPlayerWhenObjectInEquipment) {
        Player receiver{1241525, "Mo", "Do"};

        Object item{12345, "janga", {}, {}, Slot::Head};
        handler.pickupItem(player, item);
        handler.equipItem(player, item);

        ASSERT_FALSE(player.getMutableInventory().isItemInInventory(item));
        ASSERT_TRUE(player.getMutableEquipment().isItemEquipped(item));

        handler.giveItem(player, receiver, item);

        ASSERT_FALSE(player.getMutableInventory().isItemInInventory(item));
        ASSERT_FALSE(player.getMutableEquipment().isItemEquipped(item));
        EXPECT_TRUE(receiver.getMutableInventory().isItemInInventory(item));
    }
}