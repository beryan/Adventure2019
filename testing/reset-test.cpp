//
// Created by louis on 03/04/19.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Reset.h"

using model::Reset;

namespace {
    TEST(ResetTestSuite, canConstructReset) {
        std::string action = "test";
        model::ID id = 1000;
        model::ID roomId = 2000;
        int limit = 4;
        int slot = 16;

        Reset reset1{};

        Reset reset2{action, id, roomId};
        EXPECT_EQ(action, reset2.getAction());
        EXPECT_EQ(id, reset2.getId());
        EXPECT_EQ(roomId, reset2.getRoom());

        Reset reset3{action, id, limit, roomId};
        EXPECT_EQ(action, reset3.getAction());
        EXPECT_EQ(id, reset3.getId());
        EXPECT_EQ(limit, reset3.getLimit());
        EXPECT_EQ(roomId, reset3.getRoom());

        Reset reset4{action, id, limit, roomId, slot};
        EXPECT_EQ(action, reset4.getAction());
        EXPECT_EQ(id, reset4.getId());
        EXPECT_EQ(limit, reset4.getLimit());
        EXPECT_EQ(roomId, reset4.getRoom());
        EXPECT_EQ(slot, reset4.getSlot());
    }


    TEST(ResetTestSuite, canSetAction) {
        std::string action = "test";
        Reset reset{};
        reset.setAction("test");
        EXPECT_EQ(action, reset.getAction());
    }


    TEST(ResetTestSuite, canSetId) {
        model::ID id = 9999;
        Reset reset{};
        reset.setId(id);
        EXPECT_EQ(id, reset.getId());
    }


    TEST(ResetTestSuite, canSetLimit) {
        int limit = 7;
        Reset reset{};
        reset.setLimit(7);
        EXPECT_EQ(limit, reset.getLimit());
    }


    TEST(ResetTestSuite, canSetRoom) {
        model::ID roomId = 3333;
        Reset reset{};
        reset.setRoom(roomId);
        EXPECT_EQ(roomId, reset.getRoom());
    }


    TEST(ResetTestSuite, canSetSlot) {
        int slot = 4;
        Reset reset{};
        reset.setSlot(4);
        EXPECT_EQ(slot, reset.getSlot());
    }
}
