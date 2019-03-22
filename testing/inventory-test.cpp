//
// Created by jnhkm on 2019-02-26.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Inventory.h"
#include <stdlib.h>
#include <iostream>

using model::Inventory;
using model::Slot;

namespace {
    class InventoryTestSuite : public ::testing::Test {
    protected:
        Inventory inventory;

        virtual void SetUp() override {
            inventory = {};
        }
    };

    TEST_F(InventoryTestSuite, canMapInventory) {
        Object obj1{};
        Object obj2{};

        std::map<Object, int> map {
            std::make_pair(obj1, 2),
            std::make_pair(obj2, 3)
        };

        std::vector<Object> vec = {obj1, obj2};

        inventory.mapInventory(vec);
        EXPECT_EQ(map, inventory.getMappedInventory());
    }

    TEST_F(InventoryTestSuite, canAddTwoSameItemsToInventory) {
        Object obj{12, "boo", {}, {}, Slot::Head};

        inventory.addItemToInventory(obj);
        inventory.addItemToInventory(obj);

        EXPECT_EQ(2, inventory.getMappedInventory().at(obj));
    }

    TEST_F(InventoryTestSuite, canCompletelyRemoveItemFromInventory) {
        Object obj{12, "boo", {}, {}, Slot::Head};
        unsigned int expected = 0;

        inventory.addItemToInventory(obj);

        Object dropped = inventory.removeItemFromInventory(obj);

        EXPECT_EQ(expected, inventory.getMappedInventory().count(obj));
        EXPECT_EQ(dropped, obj);
    }

    TEST_F(InventoryTestSuite, canAddTwoSameItemsToInventoryAndRemoveOne) {
        Object obj{12, "boo", {}, {}, Slot::Head};

        inventory.addItemToInventory(obj);
        inventory.addItemToInventory(obj);

        Object dropped = inventory.removeItemFromInventory(obj);

        EXPECT_EQ(1, inventory.getMappedInventory().at(obj));
        EXPECT_EQ(dropped, obj);
    }

    TEST_F(InventoryTestSuite, canCheckExistingItemIsInInventory) {
        Object obj{12, "boo", {}, {}, Slot::Head};

        inventory.addItemToInventory(obj);

        EXPECT_TRUE(inventory.isItemInInventory(obj));
    }

    TEST_F(InventoryTestSuite, canCheckNotExistingItemIsNotInInventory) {
        Object obj{12, "boo", {}, {}, Slot::Head};

        inventory.addItemToInventory(obj);

        inventory.removeItemFromInventory(obj);

        EXPECT_FALSE(inventory.isItemInInventory(obj));
    }
}