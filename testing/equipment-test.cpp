
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Equipment.h"
#include <stdlib.h>
#include <iostream>

using model::Equipment;
using model::Slot;

constexpr int EXPECTED_FULL_OFFENCE_VALUE = 5;
constexpr int EXPECTED_FULL_ARMOUR_VALUE = 9;
constexpr double EXPECTED_FULL_CRITICAL_VALUE = 0.05;
constexpr double EXPECTED_FULL_DODGE_VALUE = 0.05;

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

    TEST_F(EquipmentTestSuite, canGetOffenceValue) {
        Object weapon{};
        weapon.setSlot(Slot::Weapon);

        equipment.equipItem(weapon);

        ASSERT_TRUE(equipment.isSlotOccupied(weapon.getSlot()));
        ASSERT_TRUE(equipment.isItemEquipped(weapon));
        EXPECT_EQ(EXPECTED_FULL_OFFENCE_VALUE, equipment.getOffenceValue());
    }

    TEST_F(EquipmentTestSuite, canGetDefenceValue) {
        std::vector<Slot> slots = {Slot::Head, Slot::Shoulders, Slot::Chest, Slot::Hands, Slot::Legs};

        for (const auto &slot : slots) {
            Object gear{};
            gear.setSlot(slot);
            equipment.equipItem(gear);
            ASSERT_TRUE(equipment.isSlotOccupied(gear.getSlot()));
            ASSERT_TRUE(equipment.isItemEquipped(gear));
        }

        EXPECT_EQ(EXPECTED_FULL_ARMOUR_VALUE, equipment.getDefenceValue());
    }

    TEST_F(EquipmentTestSuite, canGetCriticalValue) {
        std::vector<Slot> slots = {Slot::Weapon, Slot::Back};

        for (const auto &slot : slots) {
            Object gear{};
            gear.setSlot(slot);
            equipment.equipItem(gear);
            ASSERT_TRUE(equipment.isSlotOccupied(gear.getSlot()));
            ASSERT_TRUE(equipment.isItemEquipped(gear));
        }

        auto expected = static_cast<int>(EXPECTED_FULL_CRITICAL_VALUE * 100);
        auto result = static_cast<int>(equipment.getCriticalValue() * 100);

        EXPECT_EQ(expected, result);
    }

    TEST_F(EquipmentTestSuite, canGetDodgeValue) {
        std::vector<Slot> slots = {Slot::Feet, Slot::Back};

        for (const auto &slot : slots) {
            Object gear{};
            gear.setSlot(slot);
            equipment.equipItem(gear);
            ASSERT_TRUE(equipment.isSlotOccupied(gear.getSlot()));
            ASSERT_TRUE(equipment.isItemEquipped(gear));
        }

        auto expected = static_cast<int>(EXPECTED_FULL_DODGE_VALUE * 100);
        auto result = static_cast<int>(equipment.getDodgeValue() * 100);

        EXPECT_EQ(expected, result);
    }
}