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

    void Room::addDoor(Door door) {
      doors.push_back(std::move(door));
    }

    void Room::addNPC(NPC npc) {
      npcs.push_back(std::move(npc));
    }

    void Room::addObject(Object object) {
      objects.push_back(std::move(object));
    }

    void Room::addPlayerToRoom(model::ID playerId) {
      playersInRoom.push_back(playerId);
    }

    void Room::removePlayerFromRoom(model::ID playerId) {
      playersInRoom.erase(std::remove(playersInRoom.begin(), playersInRoom.end(), playerId), playersInRoom.end());
    }

    //create temporary room to showcase functionality based on provided mirkwood file
    void Room::createStub() {
      this->id = 8800;
      this->name = "Entrance to Mirkwood";
      this->desc = {"You have entered the quasi-magical Elven forest of Mirkwood, populated by",
      "Sylvan Elves and their mysterious allies. Strangers are frowned upon here,",
      "so be on your guard. It is rumored that many different races of Elves",
      "actually inhabit this forest, in various parts and sometimes they are",
      "carefully hidden, away from the prying eyes of strangers."};
      this->doors.push_back({"north",8801});
      this->doors.push_back({"south", 8855, {"You see an old archway.", "It is covered in spider webs."}, {"archway","webs"}});
    }

    bool Room::operator==(const Room& Room) const {
      return this->id == Room.getId();
    }

    //print object
    std::ostream& operator<<(std::ostream& os, const Room& rhs) {
      os << "\n*****" << rhs.name << "*****\n";

      if (rhs.desc.size() != 0) {
        for (std::string s : rhs.desc) {
      		os << s << std::endl;
      	}
      }

    	return os;
    }

}
