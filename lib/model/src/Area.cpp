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

    void Area::addNpc(const NPC &npc) {
        this->npcs.push_back(npc);
    }

    void Area::addObject(const Object &object) {
        this->objects.push_back(object);
    }

    void Area::addReset(const Reset &reset) {
        this->resets.push_back(reset);
    }

    std::vector<Room>::iterator Area::findRoomById(model::ID roomID){
        return std::find_if(this->rooms.begin(), this->rooms.end(),
                            [roomID](const Room & room) -> bool { return room.getId() == roomID ; });
    }

    std::vector<NPC>::iterator Area::findNpcById(model::ID npcID){
        return std::find_if(this->npcs.begin(), this->npcs.end(),
                            [npcID](const NPC & npc) -> bool { return npc.getId() == npcID ; });
    }

    std::vector<Object>::iterator Area::findObjectById(model::ID objectID){
        return std::find_if(this->objects.begin(), this->objects.end(),
                            [objectID](const Object & object) -> bool { return object.getId() == objectID ; });
    }

    bool Area::npcExists(const model::ID &npcId) {
        return findNpcById(npcId) != this->npcs.end();
    }

    bool Area::objectExists(const model::ID &objectId) {
        return findObjectById(objectId) != this->objects.end();
    }

    void Area::addNpcsToRooms() {
        for(const Reset &r : this->resets) {
            if(r.getAction() == "npc") {
                auto room = findRoomById(r.getRoom());
                auto npc = findNpcById(r.getId());

                if(room != this->rooms.end() && npc != this->npcs.end()) {
                    room->addNPC(*npc);
                }

            }
        }
    }

    void Area::addObjectsToRooms() {
        for(Reset r : this->resets) {
            if(r.getAction() == "object") {
                auto room = findRoomById(r.getRoom());
                auto object = findObjectById(r.getId());

                if(room != this->rooms.end() && object != this->objects.end()) {
                    room->addObject(*object);
                }

            }
        }
    }

    bool Area::operator==(const Area& area) const {
        return this->name == area.getName();
    }

    //print area
    std::ostream& operator<<(std::ostream& os, const Area& area) {
        os << area.name << ":\n";

        if (area.rooms.size()) {
            for (const auto &room : area.rooms) {
                os << "- " << room.getId() << ". " << room.getName() << std::endl;
            }
        }

        return os;
    }

}
