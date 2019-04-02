//
// Created by arehal on 1/18/19.
//

#include <iostream>
#include <boost/filesystem.hpp>

#include "WorldHandler.h"
#include "DataManager.h"
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/case_conv.hpp>

using handler::WorldHandler;
using json = nlohmann::json;

constexpr auto DATA_JSON_PATH = "lib/data/mirkwood.json";
constexpr auto LOAD_FILE_PATH = "lib/data/saveFile.json";

namespace handler {

    WorldHandler::WorldHandler() {
        //be sure to check STARTING_LOCATION in Player.h
        this->world = World();

        // load save file if it exists, otherwise load a default area file
        if (boost::filesystem::exists( LOAD_FILE_PATH )) {
            std::cout << "Loaded save file" << std::endl;
            auto savedAreas = DataManager::ParseWorldFile(LOAD_FILE_PATH);
            for(Area& area: savedAreas) {
                resetHandler.loadSavedResets(area);
            }
            this->world.setAreas(savedAreas);

        } else {
            std::cout << "Loaded default area file" << std::endl;
            this->world.addArea(DataManager::ParseDataFile(DATA_JSON_PATH));
            reset();
        }
    }

    World WorldHandler::getWorld() const {
        return world;
    }

    void WorldHandler::setWorld(const World &world) {
        this->world = world;
    }

    bool WorldHandler::roomExists(const model::ID &roomId) {
        bool found = false;
        for (Area &area : this->world.getAreas()) {
            std::vector<Room>& rooms = area.getRooms();
            auto it = std::find_if(rooms.begin(), rooms.end(), [&roomId](const Room &room) {return room.getId() == roomId;});
            if (it != rooms.end()) {
                found = true;
            }
        }
        return found;
    }

    Room& WorldHandler::findRoom(const model::ID &roomId) {
        for (Area &area : this->world.getAreas()) {
            std::vector<Room>& rooms = area.getRooms();
            auto it = std::find_if(rooms.begin(), rooms.end(), [&roomId](const Room &room) {return room.getId() == roomId;});
            if (it != rooms.end()) {
                return *it;
            }
        }
        throw std::runtime_error("Error: tried to find room that does not exist");
    }

    Area& WorldHandler::findArea(const model::ID &roomId) {
        for (Area &area : this->world.getAreas()) {
            std::vector<Room>& rooms = area.getRooms();
            auto it = std::find_if(rooms.begin(), rooms.end(), [&roomId](const Room &room) {return room.getId() == roomId;});
            if (it != rooms.end()) {
                return area;
            }
        }
        throw std::runtime_error("Error: tried to find area that does not exist");
    }

    bool WorldHandler::isValidDirection(const model::ID &roomId, const std::string &dir) {
        Room& room = findRoom(roomId);
        return room.isValidDirection(dir);
    }

    model::ID WorldHandler::getDestination(const model::ID &roomId, const std::string &dir) {
        Room& room = findRoom(roomId);
        return room.getDestination(dir);
    }

    void WorldHandler::addPlayer(const model::ID &roomId, const model::ID &playerId) {
        Room& room = findRoom(roomId);
        room.addPlayerToRoom(playerId);
    }

    void WorldHandler::removePlayer(const model::ID &roomId, const model::ID &playerId) {
        Room& room = findRoom(roomId);
        room.removePlayerFromRoom(playerId);
    }

    void WorldHandler::movePlayer(const model::ID &playerId, const model::ID &sourceId, const model::ID &destinationId) {
        removePlayer(sourceId, playerId);
        addPlayer(destinationId, playerId);
    }

    void WorldHandler::addItem(const model::ID &roomId, const Object &item) {
        Room& room = findRoom(roomId);
        room.addObject(item);
    }

    void WorldHandler::removeItem(const model::ID &roomId, const model::ID &objectId) {
        Room& room = findRoom(roomId);
        room.removeObject(objectId);
    }

    bool WorldHandler::canGive(const model::ID &roomId, const model::ID &playerId) {
        Room& room = findRoom(roomId);
        auto players = room.getPlayersInRoom();
        auto it = std::find(players.begin(), players.end(), playerId);
        return (it != players.end());
    }

    std::vector<model::ID> WorldHandler::getNearbyPlayerIds(const model::ID &roomId) {
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

    bool WorldHandler::createArea(const std::string &name) {
        //acreate [name]
        if (!name.empty()) {
            auto area = Area(name);
            this->world.addArea(area);
        }
        return !name.empty();
    }

    bool WorldHandler::createRoom(const std::vector<std::string> &arguments) {
        //rcreate [anum] [id] [name]
        bool canCreate = false;
        if (arguments.size() > 2 && isNum(arguments.at(0)) && isNum(arguments.at(1))) {
            unsigned int index = std::stoi(arguments.at(0)) - 1;
            unsigned int roomId = std::stoi(arguments.at(1));
            canCreate = (index < this->world.getAreas().size() && !roomExists(roomId));
            if (canCreate) {
                std::string name = boost::algorithm::join(std::vector<std::string>(arguments.begin()+2, arguments.end()), " ");
                auto room = Room(roomId, name);
                this->world.getAreas().at(index).addRoom(room);
            }
        }
        return canCreate;
    }

    bool WorldHandler::createObject(const model::ID &roomId, const std::vector<std::string> &arguments) {
        //ocreate [id] [short description]
        bool canCreate = false;
        if (arguments.size() > 1 && isNum(arguments.at(0))) {
            unsigned int id = std::stoi(arguments.at(0));
            auto &area = findArea(roomId);
            canCreate = !area.objectExists(id);
            if (canCreate) {
                std::string shortDesc = boost::algorithm::join(std::vector<std::string>(arguments.begin()+1, arguments.end()), " ");
                auto object = Object(id, shortDesc);
                area.addObject(object);
            }
        }
        return canCreate;
    }

    bool WorldHandler::createNpc(const model::ID &roomId, const std::vector<std::string> &arguments) {
        //ncreate [id] [short description]
        bool canCreate = false;
        if (arguments.size() > 1 && isNum(arguments.at(0))) {
            unsigned int id = std::stoi(arguments.at(0));
            auto &area = findArea(roomId);
            canCreate = !area.npcExists(id);
            if (canCreate) {
                std::string shortDesc = boost::algorithm::join(std::vector<std::string>(arguments.begin()+1, arguments.end()), " ");
                auto npc = NPC(id, shortDesc);
                area.addNpc(npc);
            }
        }
        return canCreate;
    }

    bool WorldHandler::createObjectReset(const model::ID &roomId, const std::vector<std::string> &arguments) {
        //oreset [id]
        bool canCreate = false;
        if (arguments.size() == 1 && isNum(arguments.at(0))) {
            unsigned int id = std::stoi(arguments.at(0));
            auto &area = findArea(roomId);
            canCreate = area.objectExists(id);
            if (canCreate) {
                Reset reset = Reset();
                reset.setAction("object");
                reset.setId(id);
                reset.setRoom(roomId);
                area.addReset(reset);
            }
        }
        return canCreate;
    }

    bool WorldHandler::createNpcReset(const model::ID &roomId, const std::vector<std::string> &arguments) {
        //nreset [id] [amount]
        bool canCreate = false;
        if (arguments.size() == 2 && isNum(arguments.at(0)) && isNum(arguments.at(1))) {
            unsigned int id = std::stoi(arguments.at(0));
            unsigned int amount = std::stoi(arguments.at(1));
            auto &area = findArea(roomId);
            canCreate = area.npcExists(id);
            if (canCreate) {
                Reset reset = Reset();
                reset.setAction("npc");
                reset.setId(id);
                reset.setRoom(roomId);
                reset.setLimit(amount);
                area.addReset(reset);
            }
        }
        return canCreate;
    }

    bool WorldHandler::editArea(const model::ID &roomId, const std::vector<std::string> &arguments) {
        //aedit [field] [values]
        bool success = false;
        if (arguments.size() > 1) {
            std::string field = arguments.at(0);
            std::string value = boost::algorithm::join(std::vector<std::string>(arguments.begin()+1, arguments.end()), " ");
            if (field == "name") {
                auto &area = findArea(roomId);
                area.setName(value);
                success = true;
            }
        }
        return success;
    }

    bool WorldHandler::editRoom(const model::ID &roomId, const std::vector<std::string> &arguments) {
        //redit [field] [values]
        bool success = false;
        if (arguments.size() > 1) {
            std::string field = arguments.at(0);
            std::string value = boost::algorithm::join(std::vector<std::string>(arguments.begin()+1, arguments.end()), " ");
            auto &room = findRoom(roomId);
            if (field == "name") {
                room.setName(value);
                success = true;
            } else if (field == "desc") {
                room.setDesc({value});
                success = true;
            } else if (field == "extra" && arguments.size() >= 3) {
                //redit extra [keyword] [description]
                std::string keyword = boost::algorithm::to_lower_copy(arguments.at(1));
                std::string desc = boost::algorithm::join(std::vector<std::string>(arguments.begin()+2, arguments.end()), " ");
                room.addExtra({{keyword},{desc}});
                success = true;
            } else if (field == "door") {
                //redit door [direction] [roomId] [description]
                std::string dir = boost::algorithm::to_lower_copy(arguments.at(1));
                if (!room.isValidDirection(dir) && arguments.size() >= 3 && isNum(arguments.at(2))) {
                    int leadsTo = std::stoi(arguments.at(2));
                    model::Door door = model::Door{dir, leadsTo};
                    if (arguments.size() > 3) {
                        std::string desc = boost::algorithm::join(std::vector<std::string>(arguments.begin()+3, arguments.end()), " ");
                        door.desc = {desc};
                    }
                    room.addDoor(door);
                    success = true;
                }
            }
        }
        return success;
    }

    bool WorldHandler::editObject(const model::ID &roomId, const std::vector<std::string> &arguments) {
        //oedit [id] [field] [values]
        bool success = false;
        if (arguments.size() > 2 && isNum(arguments.at(0))) {
            unsigned int id = std::stoi(arguments.at(0));
            std::string field = arguments.at(1);
            std::string value = boost::algorithm::join(std::vector<std::string>(arguments.begin()+2, arguments.end()), " ");
            auto &area = findArea(roomId);
            if (area.objectExists(id)) {
                auto object = area.findObjectById(id);
                if (field == "short") {
                    object->setShortDescription(value);
                    success = true;
                } else if (field == "long") {
                    object->setLongDescription({value});
                    success = true;
                } else if (field == "keywords") {
                    std::vector<std::string> keywords = std::vector<std::string>(arguments.begin()+2, arguments.end());
                    for (auto &keyword : keywords) {
                        keyword = boost::algorithm::to_lower_copy(keyword);
                    }
                    object->setKeywords(keywords);
                    success = true;
                } else if (field == "slot") {
                    auto slot = model::getSlotFromString(value);
                    if (slot != model::Slot::Misc) {
                        object->setSlot(slot);
                        success = true;
                    }
                }
            }
        }
        return success;
    }

    bool WorldHandler::editNpc(const model::ID &roomId, const std::vector<std::string> &arguments) {
        //nedit [id] [field] [values]
        bool success = false;
        if (arguments.size() > 2 && isNum(arguments.at(0))) {
            unsigned int id = std::stoi(arguments.at(0));
            std::string field = arguments.at(1);
            std::string value = boost::algorithm::join(std::vector<std::string>(arguments.begin()+2, arguments.end()), " ");
            auto &area = findArea(roomId);
            if (area.npcExists(id)) {
                auto npc = area.findNpcById(id);
                if (field == "short") {
                    npc->setShortDescription(value);
                    success = true;
                } else if (field == "long") {
                    npc->setLongDescription({value});
                    success = true;
                } else if (field == "desc") {
                    npc->setDescription({value});
                    success = true;
                } else if (field == "keywords") {
                    std::vector<std::string> keywords = std::vector<std::string>(arguments.begin()+2, arguments.end());
                    for (auto &keyword : keywords) {
                        keyword = boost::algorithm::to_lower_copy(keyword);
                    }
                    npc->setKeywords(keywords);
                    success = true;
                }
            }
        }
        return success;
    }

    void WorldHandler::clear(const model::ID &roomId) {
        auto &area = findArea(roomId);
        area.removeResets(roomId);
        auto &room = findRoom(roomId);
        room.setDesc({});
        room.setDoors({});
        room.setExtras({});
        room.setObjects({});
        room.setNpcs({});
    }

    void WorldHandler::resetArea(const model::ID &roomId) {
        auto &area = findArea(roomId);
        for (auto &room : area.getRooms()) {
            room.setObjects({});
            room.setNpcs({});
        }
        resetHandler.resetArea(area);
    }

    void WorldHandler::reset() {
        for (Area& area : this->world.getAreas()) {
            resetHandler.resetArea(area);
        }
    }

    void WorldHandler::processResets() {
        if (resetHandler.isTimeToReset()) {
            this->reset();
            this->resetHandler.resetTimer();

        } else {
            resetHandler.decrementTimer();
        }
    }

    bool WorldHandler::isNum(const std::string &str) const {
        return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
    }

}
