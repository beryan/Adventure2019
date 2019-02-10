/*
* Room.cpp
*
* Class Description: A class designed to represent a room.
*
* Created on: January 18, 2019
* Author: Brittany Ryan, 301217765
*/

#include "Room.h"
#include <iostream>
#include <algorithm>

using model::Room;

namespace model {

    //constructors
    Room::Room()
      : id(-1),
        name(""),
        desc({}),
        doors({}),
        npcs({}),
        objects({}),
        playersInRoom({})
        { }

    Room::Room(model::ID id, std::string name, std::vector<std::string> desc)
      : id(id),
        name(std::move(name)),
        desc(std::move(desc)),
        doors({}),
        npcs({}),
        objects({}),
        playersInRoom({})
        { }

    Room::Room(model::ID id, std::string name, std::vector<std::string> desc, std::vector<Door> doors, std::vector<NPC> npcs, std::vector<Object> objects)
      : id(id),
        name(std::move(name)),
        desc(std::move(desc)),
        doors(std::move(doors)),
        npcs(std::move(npcs)),
        objects(std::move(objects)),
        playersInRoom({})
        { }

    //getters and setters
    model::ID Room::getId() const {
        return id;
    }

    std::string Room::getName() const {
        return name;
    }

    std::vector<std::string> Room::getDesc() const {
        return desc;
    }

    std::vector<Door> Room::getDoors() const {
        return doors;
    }

    std::vector<NPC> Room::getNpcs() const {
        return npcs;
    }

    std::vector<Object> Room::getObjects() const {
        return objects;
    }

    std::vector<model::ID> Room::getPlayersInRoom() const {
        return playersInRoom;
    }

    void Room::setId(model::ID id) {
        this->id = id;
    }

    void Room::setName(std::string name) {
        this->name = std::move(name);
    }

    void Room::setDesc(std::vector<std::string> desc) {
        this->desc = std::move(desc);
    }

    void Room::setDoors(std::vector<Door> doors) {
        this->doors = std::move(doors);
    }

    void Room::setNpcs(std::vector<NPC> npcs) {
        this->npcs = std::move(npcs);
    }

    void Room::setObjects(std::vector<Object> objects) {
        this->objects = std::move(objects);
    }

    void Room::setPlayersInRoom(std::vector<model::ID> playersInRoom) {
        this->playersInRoom = std::move(playersInRoom);
    }

    void Room::addDoor(const Door &door) {
        doors.push_back(door);
    }

    void Room::addNPC(const NPC &npc) {
        npcs.push_back(npc);
    }

    void Room::addObject(const Object &object) {
        objects.push_back(object);
    }

    void Room::addPlayerToRoom(const model::ID &playerId) {
        auto it = std::find(this->playersInRoom.begin(), this->playersInRoom.end(), playerId);
        if (it == this->playersInRoom.end()) {
            playersInRoom.push_back(playerId);
        }
    }

    void Room::removePlayerFromRoom(const model::ID &playerId) {
        playersInRoom.erase(std::remove(playersInRoom.begin(), playersInRoom.end(), playerId), playersInRoom.end());
    }

    bool Room::isValidDirection(const std::string &dir) {
        auto it = std::find_if(this->doors.begin(), this->doors.end(), [&dir](const Door &door) {return door.dir == dir;});
        return (it != this->doors.end());
    }

    model::ID Room::getDestination(const std::string &dir) {
        auto it = std::find_if(this->doors.begin(), this->doors.end(), [&dir](const Door &door) {return door.dir == dir;});
        if (it != this->doors.end()) {
            return it->leadsTo;
        }
        throw std::runtime_error("Tried to get destination with illegal direction");
    }

    std::vector<model::ID> Room::getNearbyRoomIds() {
        std::vector<model::ID> ids;
        for (Door door : this->doors) {
            ids.push_back(door.leadsTo);
        }
        return ids;
    }

    bool Room::operator==(const Room& Room) const {
        return this->id == Room.getId();
    }

    //print door
    std::ostream& operator<<(std::ostream& os, const Door& rhs) {
        os << rhs.dir;
        if (rhs.desc.size() > 0) {
            os << ": ";
            for (std::string s : rhs.desc) {
                os << s << std::endl;
            }
        } else {
            os << std::endl;
        }

        return os;
    }

    //print room
    std::ostream& operator<<(std::ostream& os, const Room& rhs) {
        os << "\n***" << rhs.name << "***\n";

        if (rhs.desc.size() > 0) {
            for (std::string s : rhs.desc) {
                os << s << std::endl;
            }
        }

        for (auto door : rhs.doors) {
            os << door;
        }

        return os;
    }

}
