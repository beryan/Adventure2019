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
#include <algorithm>

using model::Area;

namespace model {

    //constructors
    Area::Area()
        : name(""),
          rooms({})
          { }

    Area::Area(std::string name)
        : name(std::move(name)),
          rooms({})
          { }

    Area::Area(std::string name, std::vector<Room> rooms)
        : name(std::move(name)),
          rooms(std::move(rooms))
          { }

    //getters and setters
    std::string Area::getName() const {
        return name;
    }

    std::vector<Room>& Area::getRooms() {
        return rooms;
    }

    std::vector<NPC> Area::getNpcs() const {
        return npcs;
    }

    std::vector<Object> Area::getObjects() const {
        return objects;
    }

    std::vector<Reset> Area::getResets() const {
        return resets;
    }

    void Area::setName(const std::string &name) {
        this->name = name;
    }

    void Area::setRooms(const std::vector<Room> &rooms) {
        this->rooms = rooms;
    }

    void Area::setNpcs(const std::vector<NPC> &npcs) {
        this->npcs = npcs;
    }

    void Area::setObjects(const std::vector<Object> &objects) {
        this->objects = objects;
    }

    void Area::setResets(const std::vector<Reset> &resets) {
        this->resets = resets;
    }

    void Area::addRoom(const Room &room) {
        this->rooms.push_back(room);
    }

    void Area::addNPC(const NPC &npc) {
        this->npcs.push_back(npc);
    }

    void Area::addObject(const Object &object) {
        this->objects.push_back(object);
    }

    void Area::addReset(const Reset &reset) {
        this->resets.push_back(reset);
    }

    bool Area::operator==(const Area& area) const {
        return this->name == area.getName();
    }

    //print object
    std::ostream& operator<<(std::ostream& os, const Area& area) {
        os << "Area: " << area.name << std::endl;

        if (area.rooms.size()) {
            for (const auto &room : area.rooms) {
                os << room;
            }
        }

        return os;
    }

}
