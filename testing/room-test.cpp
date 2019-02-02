//
// Created by Brittany Ryan on 2019-02-01.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Room.h"

using model::Room;

namespace {

    TEST(RoomTestSuite, canConstructRoom) {
        int expected_id = 12345;
        std::string expected_name = "name";
        std::vector<std::string> expected_desc = {"desc1","desc2"};

        Room room{expected_id, expected_name, expected_desc};

        EXPECT_EQ(room.getId(), expected_id);
        EXPECT_EQ(room.getName(), expected_name);
        EXPECT_EQ(room.getDesc(), expected_desc);
    }

}
