//
// Created by Stephen Wanhella on 2019-02-14.
//

#include <WorldHandler.h>
#include "gtest/gtest.h"
#include "Character.h"

namespace {
    model::ID getFirstRoom(const model::WorldHandler  &wh) {
        EXPECT_FALSE(wh.getWorld().getAreas().empty());
        auto area = wh.getWorld().getAreas()[0];
        EXPECT_FALSE(wh.getWorld().getAreas()[0].getRooms().empty());
        auto room = area.getRooms()[0];
        return room.getId();
    }

    TEST(WorldHandlerTestSuite, canMakeWorld) {
        model::WorldHandler worldHandler;
    }

    TEST(WorldHandlerTestSuite, canLoadAreas) {
        model::WorldHandler worldHandler;
        EXPECT_FALSE(worldHandler.getWorld().getAreas().empty());
    }

    TEST(WorldHandlerTestSuite, canAddAndRemovePlayerToRoom) {
        model::WorldHandler worldHandler;
        auto roomId = getFirstRoom(worldHandler);
        model::ID playerId = 1234;
        worldHandler.addPlayer(playerId, roomId);

        auto room = worldHandler.findRoom(roomId);
        auto playersInRoom = room.getPlayersInRoom();
        auto it = std::find(playersInRoom.begin(), playersInRoom.end(), playerId);
        EXPECT_NE(it, playersInRoom.end());

        worldHandler.removePlayer(playerId, roomId);

        // have to re-find room after making changes because it's not returned by reference
        room = worldHandler.findRoom(roomId);
        playersInRoom = room.getPlayersInRoom();
        it = std::find(playersInRoom.begin(), playersInRoom.end(), playerId);
        EXPECT_EQ(it, playersInRoom.end());
    }

    TEST(WorldHandlerTestSuite, canGetAdjacentRooms) {
        model::WorldHandler worldHandler;
        auto roomId = getFirstRoom(worldHandler);
        auto doors = worldHandler.findRoom(roomId).getDoors();
        std::unordered_map<std::string, model::ID> destinations;
        for (const auto &door: doors) {
            destinations.emplace(door.dir, door.leadsTo);
        }
        for (const auto &destinationPair: destinations) {
            auto direction = destinationPair.first;
            EXPECT_TRUE(worldHandler.isValidDirection(roomId, direction));
            EXPECT_EQ(worldHandler.getDestination(roomId, direction), destinationPair.second);
        }
    }

    TEST(WorldHandlerTestSuite, canMovePlayer) {
        model::WorldHandler worldHandler;
        auto roomId = getFirstRoom(worldHandler);
        auto room = worldHandler.findRoom(roomId);
        auto adjacentRoomId = room.getDoors()[0].leadsTo;
        model::ID playerId = 1234;

        worldHandler.addPlayer(playerId, roomId);

        auto playersInRoom = worldHandler.findRoom(roomId).getPlayersInRoom();
        auto playersInAdjacentRoom = worldHandler.findRoom(adjacentRoomId).getPlayersInRoom();
        auto nearbyPlayers = worldHandler.getNearbyPlayerIds(roomId);

        bool playerInRoom = std::find(playersInRoom.begin(), playersInRoom.end(), playerId) != playersInRoom.end();
        bool playerInAdjacentRoom = std::find(playersInAdjacentRoom.begin(), playersInAdjacentRoom.end(), playerId) != playersInAdjacentRoom.end();
        bool playerNearby = std::find(nearbyPlayers.begin(), nearbyPlayers.end(), playerId) != nearbyPlayers.end();
        EXPECT_TRUE(playerInRoom);
        EXPECT_FALSE(playerInAdjacentRoom);
        EXPECT_TRUE(playerNearby);

        worldHandler.movePlayer(playerId, roomId, adjacentRoomId);

        playersInRoom = worldHandler.findRoom(roomId).getPlayersInRoom();
        playersInAdjacentRoom = worldHandler.findRoom(adjacentRoomId).getPlayersInRoom();
        nearbyPlayers = worldHandler.getNearbyPlayerIds(roomId);

        playerInRoom = std::find(playersInRoom.begin(), playersInRoom.end(), playerId) != playersInRoom.end();
        playerInAdjacentRoom = std::find(playersInAdjacentRoom.begin(), playersInAdjacentRoom.end(), playerId) != playersInAdjacentRoom.end();
        playerNearby = std::find(nearbyPlayers.begin(), nearbyPlayers.end(), playerId) != nearbyPlayers.end();
        EXPECT_FALSE(playerInRoom);
        EXPECT_TRUE(playerInAdjacentRoom);
        EXPECT_TRUE(playerNearby);
    }
}
