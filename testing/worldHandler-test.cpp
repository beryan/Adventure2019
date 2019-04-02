//
// Created by Stephen Wanhella on 2019-02-14.
//

#include "WorldHandler.h"
#include "gtest/gtest.h"
#include "Character.h"

using handler::WorldHandler;

namespace {
    class WorldHandlerTestSuite : public ::testing::Test {
    protected:
        WorldHandler worldHandler;
        model::ID roomId;
        model::ID roomId2;
        std::string dir;
        std::vector<model::ID> players1;
        std::vector<model::ID> players2;
        model::ID objectId;
        model::ID npcId;

        virtual void SetUp() override {
            roomId = 1000;
            roomId2 = 1001;
            dir = "east";
            players1 = {1,2,3};
            players2 = {4,5,6};
            objectId = 1111;
            npcId = 2222;

            Room room1 = {roomId, "room1", {"desc1"}};
            room1.addDoor({dir,roomId2});
            Room room2 = {roomId2, "room2", {"desc2"}};

            std::for_each(players1.begin(), players1.end(), [&room1](const auto &id) {room1.addPlayerToRoom(id);});
            std::for_each(players2.begin(), players2.end(), [&room2](const auto &id) {room2.addPlayerToRoom(id);});

            Area area = Area("area");
            area.addRoom(room1);
            area.addRoom(room2);
            area.addObject({objectId, "short", {"long"}, {"keyword"}, Slot::Misc});
            area.addNpc({npcId, {"keyword"}, {"desc"}, "short", {"long"}});
            area.addReset({"object",objectId,-1,roomId,-1});

            worldHandler.setWorld({{area}});
        }
    };

    TEST_F(WorldHandlerTestSuite, canMakeWorld) {
        WorldHandler handler;
    }

    TEST_F(WorldHandlerTestSuite, canLoadAreas) {
        WorldHandler handler;
        EXPECT_FALSE(handler.getWorld().getAreas().empty());
    }

    TEST_F(WorldHandlerTestSuite, canAddAndRemovePlayerFromRoom) {
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

    TEST_F(WorldHandlerTestSuite, canGetAdjacentRooms) {
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

    TEST_F(WorldHandlerTestSuite, canMovePlayer) {
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

    TEST_F(WorldHandlerTestSuite, canThrowExceptionOnInvalidRoom) {
        model::ID nonExistentRoomId = 12345678;
        ASSERT_THROW(worldHandler.findRoom(nonExistentRoomId), std::runtime_error);
    }

    TEST_F(WorldHandlerTestSuite, canAddAndRemoveItemFromRoom) {
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

    TEST_F(WorldHandlerTestSuite, canGive) {
        model::ID playerId = 1234;
        EXPECT_FALSE(worldHandler.canGive(roomId, playerId));
        worldHandler.addPlayer(roomId, playerId);
        EXPECT_TRUE(worldHandler.canGive(roomId, playerId));
    }

    TEST_F(WorldHandlerTestSuite, canGetNearbyPlayerIds) {
        auto expected_size = players1.size() + players2.size();
        auto nearby = worldHandler.getNearbyPlayerIds(roomId);
        EXPECT_EQ(expected_size, nearby.size());
    }

    TEST_F(WorldHandlerTestSuite, canCreateArea) {
        auto size = worldHandler.getWorld().getAreas().size();
        auto expected_size = size + 1;

        auto result = worldHandler.createArea("");
        EXPECT_FALSE(result);
        EXPECT_EQ(size, worldHandler.getWorld().getAreas().size());

        result = worldHandler.createArea("name");
        EXPECT_TRUE(result);
        EXPECT_EQ(expected_size, worldHandler.getWorld().getAreas().size());
    }

    TEST_F(WorldHandlerTestSuite, canCreateRoom) {
        model::ID newRoomId = 1234;

        auto tooFew = worldHandler.createRoom({});
        EXPECT_FALSE(tooFew);

        auto badFormat = worldHandler.createRoom({"a", "b", "c"});
        EXPECT_FALSE(badFormat);

        auto badArea = worldHandler.createRoom({"2", std::to_string(newRoomId), "name"});
        EXPECT_FALSE(badArea);

        auto takenId = worldHandler.createRoom({"1", std::to_string(roomId), "name"});
        EXPECT_FALSE(takenId);

        auto result = worldHandler.createRoom({"1", std::to_string(newRoomId), "name"});
        EXPECT_TRUE(result);
        EXPECT_TRUE(worldHandler.roomExists(newRoomId));
    }

    TEST_F(WorldHandlerTestSuite, canCreateObject) {
        model::ID newObjectId = 1234;
        std::string desc = "a short desc";

        auto tooFew = worldHandler.createObject(roomId, {});
        EXPECT_FALSE(tooFew);

        auto badFormat = worldHandler.createObject(roomId, {"a", "b"});
        EXPECT_FALSE(badFormat);

        auto takenId = worldHandler.createObject(roomId, {std::to_string(objectId), desc});
        EXPECT_FALSE(takenId);

        auto result = worldHandler.createObject(roomId, {std::to_string(newObjectId), desc});
        auto area = worldHandler.findArea(roomId);
        EXPECT_TRUE(result);
        ASSERT_TRUE(area.objectExists(newObjectId));
        EXPECT_EQ(desc, area.findObjectById(newObjectId)->getShortDescription());

    }

    TEST_F(WorldHandlerTestSuite, canCreateNpc) {
        model::ID newNpcId = 1234;
        std::string desc = "a desc";

        auto tooFew = worldHandler.createNpc(roomId, {});
        EXPECT_FALSE(tooFew);

        auto badFormat = worldHandler.createNpc(roomId, {"a", "b"});
        EXPECT_FALSE(badFormat);

        auto takenId = worldHandler.createNpc(roomId, {std::to_string(npcId), desc});
        EXPECT_FALSE(takenId);

        auto result = worldHandler.createNpc(roomId, {std::to_string(newNpcId), desc});
        auto area = worldHandler.findArea(roomId);
        EXPECT_TRUE(result);
        ASSERT_TRUE(area.npcExists(newNpcId));
        EXPECT_EQ(desc, area.findNpcById(newNpcId)->getShortDescription());
    }

    TEST_F(WorldHandlerTestSuite, canCreateObjectReset) {
        auto tooFew = worldHandler.createObjectReset(roomId, {});
        EXPECT_FALSE(tooFew);

        auto badFormat = worldHandler.createObjectReset(roomId, {"a"});
        EXPECT_FALSE(badFormat);

        auto badId = worldHandler.createObjectReset(roomId, {"1234"});
        EXPECT_FALSE(badId);

        auto result = worldHandler.createObjectReset(roomId, {std::to_string(objectId)});
        EXPECT_TRUE(result);
    }

    TEST_F(WorldHandlerTestSuite, canCreateNpcReset) {
        auto tooFew = worldHandler.createNpcReset(roomId, {});
        EXPECT_FALSE(tooFew);

        auto badFormat = worldHandler.createNpcReset(roomId, {std::to_string(npcId), "a"});
        EXPECT_FALSE(badFormat);

        auto badId = worldHandler.createNpcReset(roomId, {"1234", "1"});
        EXPECT_FALSE(badId);

        auto result = worldHandler.createNpcReset(roomId, {std::to_string(npcId), "1"});
        EXPECT_TRUE(result);
    }

    TEST_F(WorldHandlerTestSuite, canEditArea) {
        std::string name = "new name";

        auto tooFew = worldHandler.editArea(roomId, {});
        EXPECT_FALSE(tooFew);

        auto result = worldHandler.editArea(roomId, {"name", name});
        EXPECT_TRUE(result);
        EXPECT_EQ(name, worldHandler.findArea(roomId).getName());
    }

    TEST_F(WorldHandlerTestSuite, canEditRoom) {
        std::string name = "name";
        std::string desc = "desc";
        std::string keyword = "keyword";
        ExtraInfo extra = {{keyword}, {desc}};
        std::string direction = "up";
        model::ID destination = 1234;

        auto tooFew = worldHandler.editRoom(roomId, {});
        EXPECT_FALSE(tooFew);

        auto result = worldHandler.editRoom(roomId, {"name", name});
        EXPECT_TRUE(result);
        EXPECT_EQ(name, worldHandler.findRoom(roomId).getName());

        result = worldHandler.editRoom(roomId, {"desc", desc});
        EXPECT_TRUE(result);
        EXPECT_EQ(desc, worldHandler.findRoom(roomId).getDesc().at(0));

        result = worldHandler.editRoom(roomId, {"extra", keyword, desc});
        EXPECT_TRUE(result);
        EXPECT_EQ(extra, worldHandler.findRoom(roomId).getExtras().at(0));

        auto takenDir = worldHandler.editRoom(roomId, {"door", dir, std::to_string(destination)});
        EXPECT_FALSE(takenDir);

        result = worldHandler.editRoom(roomId, {"door", direction, std::to_string(destination), desc});
        EXPECT_TRUE(result);
        EXPECT_TRUE(worldHandler.isValidDirection(roomId, direction));
        EXPECT_EQ(destination, worldHandler.getDestination(roomId, direction));
    }

    TEST_F(WorldHandlerTestSuite, canEditObject) {
        std::string newObjectId = "1234";
        std::string shortDesc = "short";
        std::string longDesc = "long";
        std::string keyword = "keyword";
        std::string slotName = "weapon";

        auto tooFew = worldHandler.editObject(roomId, {});
        EXPECT_FALSE(tooFew);

        auto badId = worldHandler.editObject(roomId, {newObjectId, "short", shortDesc});
        EXPECT_FALSE(badId);

        auto result = worldHandler.editObject(roomId, {std::to_string(objectId), "short", shortDesc});
        auto &area = worldHandler.findArea(roomId);
        EXPECT_TRUE(result);
        EXPECT_EQ(shortDesc, area.findObjectById(objectId)->getShortDescription());

        result = worldHandler.editObject(roomId, {std::to_string(objectId), "long", longDesc});
        EXPECT_TRUE(result);
        EXPECT_EQ(longDesc, area.findObjectById(objectId)->getLongDescription().at(0));

        result = worldHandler.editObject(roomId, {std::to_string(objectId), "keywords", keyword});
        EXPECT_TRUE(result);
        EXPECT_EQ(keyword, area.findObjectById(objectId)->getKeywords().at(0));

        result = worldHandler.editObject(roomId, {std::to_string(objectId), "slot", slotName});
        model::Slot slot = area.findObjectById(objectId)->getSlot();
        EXPECT_TRUE(result);
        EXPECT_EQ(slotName, model::getStringFromSlot(slot));
    }

    TEST_F(WorldHandlerTestSuite, canEditNpc) {
        std::string newNpcId = "1234";
        std::string shortDesc = "short";
        std::string longDesc = "long";
        std::string keyword = "keyword";

        auto tooFew = worldHandler.editNpc(roomId, {});
        EXPECT_FALSE(tooFew);

        auto badId = worldHandler.editNpc(roomId, {newNpcId, "short", shortDesc});
        EXPECT_FALSE(badId);

        auto result = worldHandler.editNpc(roomId, {std::to_string(npcId), "short", shortDesc});
        auto &area = worldHandler.findArea(roomId);
        EXPECT_TRUE(result);
        EXPECT_EQ(shortDesc, area.findNpcById(npcId)->getShortDescription());

        result = worldHandler.editNpc(roomId, {std::to_string(npcId), "long", longDesc});
        EXPECT_TRUE(result);
        EXPECT_EQ(longDesc, area.findNpcById(npcId)->getLongDescription().at(0));

        result = worldHandler.editNpc(roomId, {std::to_string(npcId), "keywords", keyword});
        EXPECT_TRUE(result);
        EXPECT_EQ(keyword, area.findNpcById(npcId)->getKeywords().at(0));
    }

    TEST_F(WorldHandlerTestSuite, canClear) {
        std::string desc = "desc";
        std::string keyword = "keyword";
        worldHandler.findRoom(roomId).addExtra({{keyword}, {desc}});
        worldHandler.reset();

        EXPECT_TRUE(worldHandler.findRoom(roomId).getDesc().size());
        EXPECT_TRUE(worldHandler.findRoom(roomId).getDoors().size());
        EXPECT_TRUE(worldHandler.findRoom(roomId).getExtras().size());
        EXPECT_TRUE(worldHandler.findRoom(roomId).getObjects().size());

        worldHandler.clear(roomId);

        EXPECT_FALSE(worldHandler.findRoom(roomId).getDesc().size());
        EXPECT_FALSE(worldHandler.findRoom(roomId).getDoors().size());
        EXPECT_FALSE(worldHandler.findRoom(roomId).getExtras().size());
        EXPECT_FALSE(worldHandler.findRoom(roomId).getObjects().size());

        worldHandler.reset();

        EXPECT_FALSE(worldHandler.findRoom(roomId).getObjects().size());
    }

    TEST_F(WorldHandlerTestSuite, canReset) {
        EXPECT_FALSE(worldHandler.findRoom(roomId).getObjects().size());
        worldHandler.reset();
        EXPECT_TRUE(worldHandler.findRoom(roomId).getObjects().size());
    }
}
