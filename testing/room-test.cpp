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
        EXPECT_EQ(room.getDesc().size(), expected_desc.size());
    }

    TEST(RoomTestSuite, canAddDoor) {
      Room room{};
      size_t sizeBefore = room.getDoors().size();

      room.addDoor({});
      EXPECT_EQ(room.getDoors().size(), sizeBefore + 1);
    }

    TEST(RoomTestSuite, canAddNPC) {
      Room room{};
      size_t sizeBefore = room.getNpcs().size();

      room.addNPC(NPC{1});
      EXPECT_EQ(room.getNpcs().size(), sizeBefore + 1);
    }

    TEST(RoomTestSuite, canAddObject) {
      Room room{};
      size_t sizeBefore = room.getObjects().size();

      room.addObject({});
      EXPECT_EQ(room.getObjects().size(), sizeBefore + 1);
    }

    TEST(RoomTestSuite, canAddPlayerId) {
      Room room{};
      size_t sizeBefore = room.getPlayersInRoom().size();

      room.addPlayerToRoom(12345);
      EXPECT_EQ(room.getPlayersInRoom().size(), sizeBefore + 1);
    }

    TEST(RoomTestSuite, canRemovePlayerId) {
      Room room{};
      room.addPlayerToRoom(12345);
      size_t sizeBefore = room.getPlayersInRoom().size();

      room.removePlayerFromRoom(1);
      EXPECT_EQ(room.getPlayersInRoom().size(), sizeBefore);

      room.removePlayerFromRoom(12345);
      EXPECT_EQ(room.getPlayersInRoom().size(), sizeBefore - 1);
    }

}
