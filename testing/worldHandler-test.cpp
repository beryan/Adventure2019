//
// Created by Stephen Wanhella on 2019-02-14.
//

#include "WorldHandler.h"
#include "gtest/gtest.h"
#include "Character.h"

using handler::WorldHandler;

namespace {
    model::ID getFirstRoom(const WorldHandler &worldHandler) {
        EXPECT_FALSE(worldHandler.getWorld().getAreas().empty());
        auto area = worldHandler.getWorld().getAreas()[0];
        EXPECT_FALSE(area.getRooms().empty());
        auto room = area.getRooms()[0];
        return room.getId();
    }

    TEST(WorldHandlerTestSuite, canMakeWorld) {
        WorldHandler worldHandler;
    }

    TEST(WorldHandlerTestSuite, canLoadAreas) {
        WorldHandler worldHandler;
        EXPECT_FALSE(worldHandler.getWorld().getAreas().empty());
    }

    TEST(WorldHandlerTestSuite, canAddAndRemovePlayerFromRoom) {
        WorldHandler worldHandler;
        auto roomId = getFirstRoom(worldHandler);
        Room &room = worldHandler.findRoom(roomId);
        model::ID playerId = 1234;

        worldHandler.addPlayer(roomId, playerId);
        auto playersInRoom = room.getPlayersInRoom();
        bool isPlayerInRoom = std::find(playersInRoom.begin(), playersInRoom.end(), playerId) != playersInRoom.end();
        EXPECT_TRUE(isPlayerInRoom);

        worldHandler.removePlayer(roomId, playerId);
        playersInRoom = room.getPlayersInRoom();
        isPlayerInRoom = std::find(playersInRoom.begin(), playersInRoom.end(), playerId) != playersInRoom.end();
        EXPECT_FALSE(isPlayerInRoom);
    }

    TEST(WorldHandlerTestSuite, canGetAdjacentRooms) {
        WorldHandler worldHandler;
        auto roomId = getFirstRoom(worldHandler);
        auto doors = worldHandler.findRoom(roomId).getDoors();
        std::unordered_map<std::string, model::ID> destinations;
        for (const auto &door: doors) {
            destinations.emplace(door.dir, door.leadsTo);
        }
        for (const auto &destinationPair: destinations) {
            auto direction = destinationPair.first;
            EXPECT_TRUE(worldHandler.isValidDirection(roomId, direction));
            auto destinationId = destinationPair.second;
            EXPECT_EQ(worldHandler.getDestination(roomId, direction), destinationId);
        }
    }

    TEST(WorldHandlerTestSuite, canMovePlayer) {
        WorldHandler worldHandler;
        auto roomId = getFirstRoom(worldHandler);
        Room &room = worldHandler.findRoom(roomId);
        auto adjacentRoomId = room.getDoors()[0].leadsTo;
        model::ID playerId = 1234;

        worldHandler.addPlayer(roomId, playerId);

        auto playersInRoom = worldHandler.findRoom(roomId).getPlayersInRoom();
        auto playersInAdjacentRoom = worldHandler.findRoom(adjacentRoomId).getPlayersInRoom();
        auto nearbyPlayers = worldHandler.getNearbyPlayerIds(roomId);

        bool isPlayerInRoom = std::find(playersInRoom.begin(), playersInRoom.end(), playerId) != playersInRoom.end();
        bool isPlayerInAdjacentRoom = std::find(playersInAdjacentRoom.begin(), playersInAdjacentRoom.end(), playerId) !=
                                      playersInAdjacentRoom.end();
        bool isPlayerInNearbyRoom =
                std::find(nearbyPlayers.begin(), nearbyPlayers.end(), playerId) != nearbyPlayers.end();
        EXPECT_TRUE(isPlayerInRoom);
        EXPECT_FALSE(isPlayerInAdjacentRoom);
        EXPECT_TRUE(isPlayerInNearbyRoom);

        worldHandler.movePlayer(playerId, roomId, adjacentRoomId);

        playersInRoom = worldHandler.findRoom(roomId).getPlayersInRoom();
        playersInAdjacentRoom = worldHandler.findRoom(adjacentRoomId).getPlayersInRoom();
        nearbyPlayers = worldHandler.getNearbyPlayerIds(roomId);

        isPlayerInRoom = std::find(playersInRoom.begin(), playersInRoom.end(), playerId) != playersInRoom.end();
        isPlayerInAdjacentRoom = std::find(playersInAdjacentRoom.begin(), playersInAdjacentRoom.end(), playerId) !=
                                 playersInAdjacentRoom.end();
        isPlayerInNearbyRoom = std::find(nearbyPlayers.begin(), nearbyPlayers.end(), playerId) != nearbyPlayers.end();
        EXPECT_FALSE(isPlayerInRoom);
        EXPECT_TRUE(isPlayerInAdjacentRoom);
        EXPECT_TRUE(isPlayerInNearbyRoom);
    }

    TEST(WorldHandlerTestSuite, canThrowExceptionOnInvalidRoom) {
        WorldHandler worldHandler;
        model::ID nonExistentRoomId = 12345678;
        ASSERT_THROW(worldHandler.findRoom(nonExistentRoomId), std::runtime_error);
    }

    TEST(WorldHandlerTestSuite, canAddAndRemoveItemFromRoom) {
        WorldHandler worldHandler;
        auto roomId = getFirstRoom(worldHandler);
        auto &room = worldHandler.findRoom(roomId);
        model::ID objectId = 1234;
        std::string shortDescription = "a sock";
        std::vector<std::string> longDescription = {"a beautiful sock somebody left here"};
        std::vector<std::string> keywords = {"sock", "stinky"};
        model::Slot slot = model::Slot::Feet;
        model::Object object{objectId, shortDescription, longDescription, keywords, slot};

        worldHandler.addItem(roomId, object);
        worldHandler.addItem(roomId, object);
        auto objects = room.getObjects();
        auto findObjectById = [objectId](const Object &obj) { return obj.getId() == objectId; };

        bool isObjectInRoom = std::find_if(objects.begin(), objects.end(), findObjectById) != objects.end();
        EXPECT_TRUE(isObjectInRoom);

        auto expected_size = objects.size() - 1;
        worldHandler.removeItem(roomId, objectId);
        objects = room.getObjects();
        EXPECT_EQ(expected_size, objects.size());
        isObjectInRoom = std::find_if(objects.begin(), objects.end(), findObjectById) != objects.end();
        EXPECT_TRUE(isObjectInRoom);

        worldHandler.removeItem(roomId, objectId);
        objects = room.getObjects();
        isObjectInRoom = std::find_if(objects.begin(), objects.end(), findObjectById) != objects.end();
        EXPECT_FALSE(isObjectInRoom);
    }

    TEST(WorldHandlerTestSuite, canGive) {
        WorldHandler worldHandler;
        auto roomId = getFirstRoom(worldHandler);
        model::ID playerId = 1234;

        EXPECT_FALSE(worldHandler.canGive(roomId, playerId));

        worldHandler.addPlayer(roomId, playerId);

        EXPECT_TRUE(worldHandler.canGive(roomId, playerId));
    }
}
