/*
* Area.cpp
*
* Class Description: A class designed idOfRoomDoorLeadsTo represent an area.
*
* Created on: January 20, 2019
* Author: Brittany Ryan, 301217765
*/

#include "Area.h"
#include <iostream>

namespace model {

    Area::Area() :
        name({}),
        NPCObjects({}),
        objects({}),
        rooms({})
        {}

    Area::Area(std::string name) :
        name(std::move(name)),
        NPCObjects({}),
        objects({}),
        rooms({})
        {}

    Area::Area(
        std::string name,
        std::vector<NPC> NPCObjects,
        std::vector<Object> objects,
        std::vector<Room> rooms
    ) :
        name(std::move(name)),
        NPCObjects(std::move(NPCObjects)),
        objects(std::move(objects)),
        rooms(std::move(rooms))
        {}

    std::string Area::getName() const {
        return this->name;
    }

    void Area::setName(std::string name) {
        this->name = std::move(name);
    }

    std::vector<NPC> Area::getNPCObjects() const {
        return this->NPCObjects;
    }

    void Area::setNPCObjects(std::vector<NPC> NPCObjects) {
        this->NPCObjects = std::move(NPCObjects);
    }

    void Area::addNPCObject(NPC NPCObjectToAdd) {
        this->NPCObjects.push_back(std::move(NPCObjectToAdd));
    }

    std::vector<Object> Area::getObjects() const {
        return this->objects;
    }

    void Area::setObjects(std::vector<Object> objects) {
        this->objects = std::move(objects);
    }

    void Area::addObject(Object objectToAdd) {
        this->objects.push_back(std::move(objectToAdd));
    }

    std::vector<Room> Area::getRooms() const {
        return this->rooms;
    }

    void Area::setRooms(std::vector<Room> rooms) {
        this->rooms = std::move(rooms);
    }

    void Area::addRoom(Room roomToAdd) {
        this->rooms.push_back(std::move(roomToAdd));
    }

    std::ostream& operator<<(std::ostream& out, const Area& area) {
        out << "You are in " << area.name << std::endl;
    	return out;
    }
}