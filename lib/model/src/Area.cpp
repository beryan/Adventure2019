/*
* Area.cpp
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
    std::string Area::getName() {
        return name;
    }

    std::vector<NPC> Area::getNpcs() {
        return npcs;
    }

    std::vector<Object> Area::getObjects() {
        return objects;
    }

    std::vector<Room> Area::getRooms() {
        return rooms;
    }

    void Area::setName(std::string name) {
        this->name = std::move(name);
    }

    void Area::setNpcs(std::vector<NPC> npcs) {
        this->npcs = std::move(npcs);
    }

    void Area::setObjects(std::vector<Object> objects) {
        this->objects = std::move(objects);
    }

    void Area::setRooms(std::vector<Room> rooms) {
        this->rooms = std::move(rooms);
    }

    void Area::addNPC(NPC npc) {
        npcs.push_back(std::move(npc));
    }

    void Area::addObject(Object object) {
        objects.push_back(std::move(object));
    }

    void Area::addRoom(Room room) {
        rooms.push_back(std::move(room));
    }

    //print object
    std::ostream& operator<<(std::ostream& os, const Area& rhs) {
        os << "You are in " << rhs.name << std::endl;

        return os;
    }

}