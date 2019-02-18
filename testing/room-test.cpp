//
// Created by Brittany Ryan on 2019-02-01.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Room.h"

using model::Room;
using model::Door;

namespace {

    TEST(RoomTestSuite, canConstructRoom) {
        int expected_id = 12345;
        std::string expected_name = "name";
        std::string expected_desc1 = "desc1";
        std::string expected_desc2 = "desc2";
        std::vector<std::string> expected_desc = {expected_desc1, expected_desc2};
        Room room{expected_id, expected_name, expected_desc};

        EXPECT_EQ(expected_id, room.getId());
        EXPECT_EQ(expected_name, room.getName());
        EXPECT_EQ(expected_desc.size(), room.getDesc().size());
        EXPECT_EQ(expected_desc1, room.getDesc().at(0));
        EXPECT_EQ(expected_desc2, room.getDesc().at(1));
    }

    TEST(RoomTestSuite, canAddDoor) {
        Room room{};
        size_t expected_size = room.getDoors().size() + 1;

        room.addDoor({});
        EXPECT_EQ(expected_size, room.getDoors().size());
    }

    TEST(RoomTestSuite, canAddNPC) {
        Room room{};
        size_t expected_size = room.getNpcs().size() + 1;

        room.addNPC(NPC{1});
        EXPECT_EQ(expected_size, room.getNpcs().size());
    }

    TEST(RoomTestSuite, canAddObject) {
        Room room{};
        size_t expected_size = room.getObjects().size() + 1;

        room.addObject({});
        EXPECT_EQ(expected_size, room.getObjects().size());
    }

    TEST(RoomTestSuite, canAddPlayerId) {
        Room room{};
        size_t expected_size = room.getPlayersInRoom().size() + 1;

        room.addPlayerToRoom(12345);
        EXPECT_EQ(expected_size, room.getPlayersInRoom().size());
    }

    TEST(RoomTestSuite, canRemovePlayerId) {
        Room room{};
        room.addPlayerToRoom(12345);
        size_t initial_size = room.getPlayersInRoom().size();
        size_t expected_size = initial_size - 1;

        room.removePlayerFromRoom(1);
        EXPECT_EQ(initial_size, room.getPlayersInRoom().size());

        room.removePlayerFromRoom(12345);
        EXPECT_EQ(expected_size, room.getPlayersInRoom().size());
    }

    TEST(RoomTestSuite, canCheckValidDirection) {
        std::string dir1 = "north";
        Door door1{dir1};

        std::string dir2 = "south";

        Room room{};
        room.addDoor(door1);

        EXPECT_TRUE(room.isValidDirection(dir1));
        EXPECT_FALSE(room.isValidDirection(dir2));
    }

    TEST(RoomTestSuite, canGetDestination) {
        std::string dir1 = "north";
        model::ID expected_id1 = 1000;
        Door door1{dir1, expected_id1};

        std::string dir2 = "south";
        model::ID expected_id2 = 2000;
        Door door2{dir2, expected_id2};

        std::vector<Door> doors = {door1, door2};

        Room room{};
        room.setDoors(doors);

        EXPECT_EQ(expected_id1, room.getDestination(dir1));
        EXPECT_EQ(expected_id2, room.getDestination(dir2));
    }

    TEST(RoomTestSuite, canGetNearbyRoomIds) {
        model::ID expected_id1 = 1000;
        Door door1{"north", expected_id1};

        model::ID expected_id2 = 2000;
        Door door2{"south", expected_id2};

        std::vector<Door> doors = {door1, door2};
        std::vector<model::ID> expected_ids = {expected_id1, expected_id2};

        Room room{};
        room.setDoors(doors);

        EXPECT_EQ(expected_ids.size(), room.getNearbyRoomIds().size());
        EXPECT_EQ(expected_id1, room.getNearbyRoomIds().at(0));
        EXPECT_EQ(expected_id2, room.getNearbyRoomIds().at(1));
    }

    TEST(RoomTestSuite, canCompareRoom) {
        Room room1{};
        Room room2{};
        room1.setId(1);

        room2.setId(1);
        EXPECT_TRUE(room1 == room2);

        room2.setId(2);
        EXPECT_FALSE(room1 == room2);
    }

}
