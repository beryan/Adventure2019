//
// Created by jnhkm on 2019-02-26.
//

#include "lib/gtest/gtest.h"
#include "lib/gmock/gmock.h"
#include "Inventory.h"
#include <stdlib.h>
#include <iostream>

namespace {
    class InventoryTestSuite : public ::testing::Test {
    protected:
        Inventory inventory;

        virtual void SetUp() override {
            inventory = {};
        }
    };

    TEST_F(InventoryTestSuite, canMappInventory) {
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
}