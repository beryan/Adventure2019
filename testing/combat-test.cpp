//
// Created by jnhkm on 2019-03-11.
//

#include "lib/gtest/gtest.h"
#include "lib/gmock/gmock.h"
#include <stdlib.h>
#include "CombatHandler.h"
#include "Player.h"
#include "NPC.h"

using action::CombatHandler;
using model::Player;
using model::NPC;

namespace {
    class CombatTestSuite : public ::testing::Test {
    protected:
        constexpr static model::ID ATTACKER_ID = 12345;
        constexpr static std::string_view ATTACKER_USERNAME = "Joe";
        constexpr static std::string_view ATTACKER_PASSWORD = "mama";

        constexpr static model::ID DEFENDER_ID = 54321;
        constexpr static std::string_view DEFENDER_USERNAME = "Lugia";
        constexpr static std::string_view DEFENDER_PASSWORD = "Pokemon";

        Player attacker;
        Player defender;

        CombatHandler handler{};

        virtual void SetUp() override {
            attacker = {ATTACKER_ID, ATTACKER_USERNAME, ATTACKER_PASSWORD};
            defender = {DEFENDER_ID, DEFENDER_USERNAME, DEFENDER_PASSWORD};
        }
    };

    TEST_F(CombatTestSuite, canNotAttackTargetAfterExitingCombat) {
        handler.enterCombat(attacker, defender);
        handler.exitCombat(attacker, defender);

        handler.attack(attacker, defender);

        ASSERT_EQ(100, attacker.getHealth());
        ASSERT_EQ(100, defender.getHealth());
    }

    TEST_F(CombatTestSuite, canAttackTarget) {
        handler.enterCombat(attacker, defender);
        handler.attack(attacker, defender);

        ASSERT_EQ(100, attacker.getHealth());
        ASSERT_NE(100, defender.getHealth());
        EXPECT_EQ(90, defender.getHealth());
    }

    TEST_F(CombatTestSuite, canNotAttackTargetWithoutEnteringCombatState) {
        handler.attack(attacker, defender);

        ASSERT_EQ(100, attacker.getHealth());
        ASSERT_EQ(100, defender.getHealth());
    }

    TEST_F(CombatTestSuite, cannotLowerTargetHealthBelowMin) {
        defender.setHealth(0);

        ASSERT_EQ(0, defender.getHealth());

        handler.enterCombat(attacker, defender);
        handler.attack(attacker, defender);

        EXPECT_EQ(0, defender.getHealth());
    }

    TEST_F(CombatTestSuite, canHealTarget) {
        defender.setHealth(95);

        ASSERT_EQ(95, defender.getHealth());

        handler.heal(attacker, defender);

        ASSERT_EQ(100, attacker.getHealth());
        EXPECT_EQ(100, defender.getHealth());
    }

    TEST_F(CombatTestSuite, cannotIncreaseTargetHealthAboveMax) {
        ASSERT_EQ(100, defender.getHealth());

        handler.heal(attacker, defender);

        EXPECT_EQ(100, defender.getHealth());
    }
}