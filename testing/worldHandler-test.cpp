//
// Created by Stephen Wanhella on 2019-02-14.
//

#include <WorldHandler.h>
#include "gtest/gtest.h"
#include "Character.h"

namespace {
    TEST(WorldHandlerTestSuite, canMakeWorld) {
        model::WorldHandler worldHandler;
    }

    TEST(WorldHandlerTestSuite, canLoadJsonAreas) {
        model::WorldHandler worldHandler;
        EXPECT_FALSE(worldHandler.getWorld().getAreas().empty());
    }

    TEST(WorldHandlerTestSuite, canAddAndRemovePlayer) {
        model::WorldHandler worldHandler;

        EXPECT_FALSE(worldHandler.getWorld().getAreas().empty());
        auto area = worldHandler.getWorld().getAreas()[0];
        EXPECT_FALSE(area.getRooms().empty());
        auto room = area.getRooms()[0];
        auto roomId = room.getId();

        auto playerId = 1234;
        worldHandler.addPlayer(playerId, roomId);

        // have to re-find room after making changes because it's not returned by reference
        room = worldHandler.findRoom(roomId);
        auto playersInRoom = room.getPlayersInRoom();
        auto it = std::find(playersInRoom.begin(), playersInRoom.end(), playerId);
        EXPECT_NE(it, playersInRoom.end());

        worldHandler.removePlayer(playerId, roomId);
        room = worldHandler.findRoom(roomId);
        playersInRoom = room.getPlayersInRoom();
        it = std::find(playersInRoom.begin(), playersInRoom.end(), playerId);
        EXPECT_EQ(it, playersInRoom.end());
    }
}
