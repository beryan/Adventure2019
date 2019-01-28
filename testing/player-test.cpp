//
// Created by jnhkm on 2019-01-25.
//

#include "lib/gtest/gtest.h"
#include "lib/gmock/gmock.h"
#include "Player.h"

using model::Player;

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
}