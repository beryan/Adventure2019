/*
* Area.h
*
* Class Description: A class designed to represent an area.
*
* Created on: January 20, 2019
* Author: Brittany Ryan, 301217765
*/

#include "Area.h"
#include <iostream>

using model::Area;

namespace model {

    //constructors
    Area::Area()
      : name(""),
        npcs({}),
        objects({}),
        rooms({})
        { }

    Area::Area(std::string name)
      : name(std::move(name)),
        npcs({}),
        objects({}),
        rooms({})
        { }

    Area::Area(std::string name, std::vector<NPC> npcs, std::vector<Object> objects, std::vector<Room> rooms)
      : name(std::move(name)),
        npcs(std::move(npcs)),
        objects(std::move(objects)),
        rooms(std::move(rooms))
        { }

    //getters and setters
    std::string getName() {
      return name;
    }

    std::vector<NPC> getNpcs() {
      return npcs;
    }

    std::vector<Object> getObjects() {
      return objects;
    }

    std::vector<Room> getRooms() {
      return rooms;
    }

    void setName(std::string name) {
      this->name = std::move(name);
    }

    void setNpcs(std::vector<NPC> npcs) {
      this->npcs = std::move(npcs);
    }

    void setObjects(std::vector<Object> objects) {
      this->objects = std::move(objects);
    }

    void setRooms(std::vector<Room> rooms) {
      this->rooms = std::move(rooms);
    }

    void addNPC(NPC npc) {
      npcs.push_back(std::move(npc));
    }

    void addObject(Object object) {
      objects.push_back(std::move(object));
    }

    void addRoom(Room room) {
      room.push_back(std::move(room));
    }

    //print object
    std::ostream& operator<<(std::ostream& os, const Area& rhs) {
      os << "You are in " << rhs.name << std::endl;

    	return os;
    }

}
