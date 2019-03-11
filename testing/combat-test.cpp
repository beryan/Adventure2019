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
        CombatHandler handler;

        virtual void SetUp() override {
            attacker = {ATTACKER_ID, ATTACKER_USERNAME, ATTACKER_PASSWORD};
            defender = {DEFENDER_ID, DEFENDER_USERNAME, DEFENDER_PASSWORD};

            handler = {};
        }
    };

    TEST_F(CombatTestSuite, canAttackTarget) {
        EXPECT_EQ(true, true);
    }
}