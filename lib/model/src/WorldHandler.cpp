//
// Created by arehal on 1/18/19.
//

#include <fstream>
#include <iostream>
#include <string>
#include <PlayerHandler.h>

#include "WorldHandler.h"

const std::string USER_JSON_PATH = "lib/data/users.json";

using model::WorldHandler;

using json = nlohmann::json;

namespace model {

    WorldHandler::WorldHandler() {
        //create temporary world
        this->world = World();
        this->world.createStub();
        //parseJSON();
    }

    Room
    WorldHandler::findRoom(const model::ID &roomID) {
        for (Area area : this->world.getAreas()) {
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
    WorldHandler::movePlayer(const model::ID &playerID, const model::ID &sourceID, const model::ID &destinationID) {
        Room source = findRoom(sourceID);
        Room destination = findRoom(destinationID);
        source.removePlayerFromRoom(playerID);
        destination.addPlayerToRoom(playerID);
    }

    void WorldHandler::addUser(Player player){
        std::cout << "Adding " << player.getId() << " " << player.getUsername() << " to users" << std::endl;
        this->world.insertUser(player);
    }

    void WorldHandler::removeUser(Player player){
        this->world.getUserMap().erase(player.getId());
    }

//    void WorldHandler::addArea(Area area){
//        this->world.getAreaMap().insert(std::make_pair(area.getId(), area));
//    }
//
//    void WorldHandler::removeArea(Area area){
//        this->world.getAreaMap().erase(area.getId());
//    }

    void WorldHandler::parseJSON() {
        std::ifstream ifs(USER_JSON_PATH);

        json t = json::parse(ifs);

        json usersJson;

        for (json::iterator it = t.begin(); it != t.end(); ++it) {
            if(it.key() == "USERS"){
                usersJson = it.value();
            }
        }

        auto players = model::PlayerHandler::parseJsonUsers(usersJson);
        for (const auto &player : players) {
            addUser(player);
        }
        world.printUsers();
    }
}
