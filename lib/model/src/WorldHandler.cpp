//
// Created by arehal on 1/18/19.
//

#include <iostream>

#include "WorldHandler.h"

using model::WorldHandler;

using json = nlohmann::json;

namespace model {

    WorldHandler::WorldHandler() {
        //create temporary world
        this->world = World();
        this->world.createStub();
    }

    World
    WorldHandler::getWorld() const {
        return world;
    }

    Room
    WorldHandler::findRoom(const model::ID &roomID) {
        for (const Area &area : this->world.getAreas()) {
            std::vector<Room> rooms = area.getRooms();
            auto it = std::find_if(rooms.begin(), rooms.end(), [&roomID](const Room &room) {return room.getId() == roomID;});
            if (it != rooms.end()) {
                return *it;
            }
        }
        throw std::runtime_error("Error: tried to find room that does not exist");
    }

    bool
    WorldHandler::isValidDirection(const model::ID &roomID, const std::string &dir) {
        Room room = findRoom(roomID);
        return room.isValidDirection(dir);
    }

    model::ID
    WorldHandler::getDestination(const model::ID &roomID, const std::string &dir) {
        Room room = findRoom(roomID);
        return room.getDestination(dir);
    }

    void
    WorldHandler::addPlayer(const model::ID &playerID, const model::ID &roomID) {
        this->world.addPlayer(playerID, roomID);
    }

    void
    WorldHandler::removePlayer(const model::ID &playerID, const model::ID &roomID) {
        this->world.removePlayer(playerID, roomID);
    }

    void
    WorldHandler::movePlayer(const model::ID &playerID, const model::ID &sourceID, const model::ID &destinationID) {
        this->world.removePlayer(playerID, sourceID);
        this->world.addPlayer(playerID, destinationID);
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
