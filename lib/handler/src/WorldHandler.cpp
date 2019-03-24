//
// Created by arehal on 1/18/19.
//

#include <iostream>

#include "WorldHandler.h"
#include "DataManager.h"

using handler::WorldHandler;
using json = nlohmann::json;

const std::string DATA_JSON_PATH = "lib/data/mirkwood.json";

namespace handler {

    WorldHandler::WorldHandler() {
        //create temporary world
        this->world = World();
        this->world.createStub();
        //this->world.addArea(DataManager::ParseDataFile(DATA_JSON_PATH));
        resetAreas();
    }

    World
    WorldHandler::getWorld() const {
        return world;
    }

    void
    WorldHandler::setWorld(World &world) {
        this->world = world;
    }

    Room&
    WorldHandler::findRoom(const model::ID &roomId) {
        for (Area &area : this->world.getAreas()) {
            std::vector<Room>& rooms = area.getRooms();
            auto it = std::find_if(rooms.begin(), rooms.end(), [&roomId](const Room &room) {return room.getId() == roomId;});
            if (it != rooms.end()) {
                return *it;
            }
        }
        throw std::runtime_error("Error: tried to find room that does not exist");
    }

    bool
    WorldHandler::isValidDirection(const model::ID &roomId, const std::string &dir) {
        Room& room = findRoom(roomId);
        return room.isValidDirection(dir);
    }

    model::ID
    WorldHandler::getDestination(const model::ID &roomId, const std::string &dir) {
        Room& room = findRoom(roomId);
        return room.getDestination(dir);
    }

    void
    WorldHandler::addPlayer(const model::ID &roomId, const model::ID &playerId) {
        Room& room = findRoom(roomId);
        room.addPlayerToRoom(playerId);
    }

    void
    WorldHandler::removePlayer(const model::ID &roomId, const model::ID &playerId) {
        Room& room = findRoom(roomId);
        room.removePlayerFromRoom(playerId);
    }

    void
    WorldHandler::movePlayer(const model::ID &playerId, const model::ID &sourceId, const model::ID &destinationId) {
        removePlayer(sourceId, playerId);
        addPlayer(destinationId, playerId);
    }

    void
    WorldHandler::addItem(const model::ID &roomId, const Object &item) {
        Room& room = findRoom(roomId);
        room.addObject(item);
    }

    void
    WorldHandler::removeItem(const model::ID &roomId, const model::ID &objectId) {
        Room& room = findRoom(roomId);
        room.removeObject(objectId);
    }

    bool
    WorldHandler::canGive(const model::ID &roomId, const model::ID &playerId) {
        Room& room = findRoom(roomId);
        auto players = room.getPlayersInRoom();
        auto it = std::find(players.begin(), players.end(), playerId);
        return (it != players.end());
    }

    void WorldHandler::resetAreas(){
        for(Area& a : this->world.getAreas()){
            resetHandler.resetArea(a);
        }
    }

    std::vector<model::ID>
    WorldHandler::getNearbyPlayerIds(const model::ID &roomId) {
        Room room = findRoom(roomId);
        std::vector<model::ID> playerIds = room.getPlayersInRoom();
        auto nearbyRoomIds = room.getNearbyRoomIds();
        for (const auto &nearbyRoomId : nearbyRoomIds) {
            Room nearbyRoom = findRoom(nearbyRoomId);
            auto playersInNearbyRoom = nearbyRoom.getPlayersInRoom();
            playerIds.insert(playerIds.end(), playersInNearbyRoom.begin(), playersInNearbyRoom.end());
        }
        return playerIds;
    }

}
