
#include "lib/gtest/gtest.h"
#include "lib/gmock/gmock.h"
#include "Equipment.h"
#include <stdlib.h>
#include <iostream>

using model::Equipment;
using model::Slot;

namespace {
    class EquipmentTestSuite : public ::testing::Test {
    protected:
        Equipment equipment{};
    };

    TEST_F(EquipmentTestSuite, canMapEquipment) {
        Object obj1{};
        Object obj2{};

        obj1.setSlot(Slot::Head);
        obj2.setSlot(Slot::Chest);

        std::unordered_map<Slot, Object> map {
                std::make_pair(obj1.getSlot(), obj1),
                std::make_pair(obj2.getSlot(), obj2)
        };

        std::vector<Object> vec = {obj1, obj2};

        equipment.mapEquipment(vec);

        EXPECT_EQ(map, equipment.getMappedEquipment());
    }

    TEST_F(EquipmentTestSuite, canGetVectorEquipment) {
        Object obj1{};
        Object obj2{};

        obj1.setSlot(Slot::Head);
        obj2.setSlot(Slot::Chest);

        std::vector<Object> vec = {obj1, obj2};

        equipment.mapEquipment(vec);

        EXPECT_EQ(vec, equipment.getVectorEquipment());
    }

    TEST_F(EquipmentTestSuite, canEquipItem) {
        Object obj{};
        obj.setSlot(Slot::Chest);

        equipment.equipItem(obj);

        EXPECT_TRUE(equipment.isSlotOccupied(obj.getSlot()));
        EXPECT_TRUE(equipment.isItemEquipped(obj));
    }

    TEST_F(EquipmentTestSuite, canUnequipByItem) {
        Object obj{};
        obj.setSlot(Slot::Chest);

        equipment.equipItem(obj);
        equipment.unequipItem(obj);

        EXPECT_FALSE(equipment.isSlotOccupied(obj.getSlot()));
        EXPECT_FALSE(equipment.isItemEquipped(obj));
    }

    TEST_F(EquipmentTestSuite, canUnequipBySlot) {
        Object obj{};
        obj.setSlot(Slot::Chest);

        equipment.equipItem(obj);
        equipment.unequipSlot(obj.getSlot());

        EXPECT_FALSE(equipment.isSlotOccupied(obj.getSlot()));
        EXPECT_FALSE(equipment.isItemEquipped(obj));
    }
}