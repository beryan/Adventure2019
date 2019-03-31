//
// Created by Brittany Ryan on 2019-01-18.
//

#include "Room.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <boost/algorithm/string/join.hpp>

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
          playersInRoom({}),
          extras({})
          { }

    Room::Room(model::ID id, std::string name)
        : id(id),
          name(std::move(name)),
          desc({}),
          doors({}),
          npcs({}),
          objects({}),
          playersInRoom({}),
          extras({})
          { }

    Room::Room(model::ID id, std::string name, std::vector<std::string> desc)
        : id(id),
          name(std::move(name)),
          desc(std::move(desc)),
          doors({}),
          npcs({}),
          objects({}),
          playersInRoom({}),
          extras({})
          { }

    Room::Room(model::ID id, std::string name, std::vector<std::string> desc, std::vector<Door> doors, std::vector<NPC> npcs, std::vector<Object> objects)
        : id(id),
          name(std::move(name)),
          desc(std::move(desc)),
          doors(std::move(doors)),
          npcs(std::move(npcs)),
          objects(std::move(objects)),
          playersInRoom({}),
          extras({})
          { }

    //getters and setters
    model::ID Room::getId() const {
        return this->id;
    }

    std::string Room::getName() const {
        return this->name;
    }

    std::vector<std::string> Room::getDesc() const {
        return this->desc;
    }

    std::vector<Door> Room::getDoors() const {
        return this->doors;
    }

    std::vector<NPC> Room::getNpcs() const {
        return this->npcs;
    }

    std::vector<Object> Room::getObjects() const {
        return this->objects;
    }

    std::vector<model::ID> Room::getPlayersInRoom() const {
        return this->playersInRoom;
    }

    std::vector<ExtraInfo> Room::getExtras() const {
        return this->extras;
    }

    void Room::setId(const model::ID &id) {
        this->id = id;
    }

    void Room::setName(const std::string &name) {
        this->name = name;
    }

    void Room::setDesc(const std::vector<std::string> &desc) {
        this->desc = desc;
    }

    void Room::setDoors(const std::vector<Door> &doors) {
        this->doors = doors;
    }

    void Room::setNpcs(const std::vector<NPC> &npcs) {
        this->npcs = npcs;
    }

    void Room::setObjects(const std::vector<Object> &objects) {
        this->objects = objects;
    }

    void Room::setPlayersInRoom(const std::vector<model::ID> &playersInRoom) {
        this->playersInRoom = playersInRoom;
    }

    void Room::setExtras(const std::vector<ExtraInfo> &extras) {
        this->extras = extras;
    }

    void Room::addDoor(const Door &door) {
        this->doors.push_back(door);
    }

    void Room::addNPC(const NPC &npc) {
        this->npcs.push_back(npc);
    }

    void Room::addObject(const Object &object) {
        this->objects.push_back(object);
    }

    void Room::addPlayerToRoom(const model::ID &playerId) {
        auto it = std::find(this->playersInRoom.begin(), this->playersInRoom.end(), playerId);
        if (it == this->playersInRoom.end()) {
            this->playersInRoom.push_back(playerId);
        }
    }

    void Room::addExtra(const ExtraInfo &extra) {
        this->extras.push_back(extra);
    }

    void Room::removeObject(const model::ID &objectId) {
        auto equal = [&objectId](const Object &obj) {return obj.getId() == objectId;};
        objects.erase(std::remove_if(objects.begin(), objects.end(), equal));
    }

    void Room::removePlayerFromRoom(const model::ID &playerId) {
        playersInRoom.erase(std::remove(playersInRoom.begin(), playersInRoom.end(), playerId), playersInRoom.end());
    }

    bool Room::isValidDirection(const std::string &dir) const {
        auto it = std::find_if(this->doors.begin(), this->doors.end(), [&dir](const Door &door) {return door.dir == dir;});
        return (it != this->doors.end());
    }

    model::ID Room::getDestination(const std::string &dir) const {
        model::ID id = -1;
        auto it = std::find_if(this->doors.begin(), this->doors.end(), [&dir](const Door &door) {return door.dir == dir;});
        if (it != this->doors.end()) {
            id = it->leadsTo;
        }
        return id;
    }

    std::vector<model::ID> Room::getNearbyRoomIds() const {
        std::vector<model::ID> ids;
        for (const auto &door : this->doors) {
            ids.push_back(door.leadsTo);
        }
        return ids;
    }

    int Room::countNpcById(const model::ID& npcID) const {
        return std::count_if(this->npcs.begin(), this->npcs.end(),
                          [&npcID](const NPC & npc) {return npc.getId() == npcID;});
    }

    int Room::countObjectById(const model::ID& objectID) const {
        return std::count_if(this->objects.begin(), this->objects.end(),
                          [&objectID](const Object & object) {return object.getId() == objectID;});
    }

    bool Room::operator==(const Room& Room) const {
        return this->id == Room.getId();
    }

    std::string Room::descToString() const {
        std::ostringstream os;
        if (!this->desc.empty()) {
            os << std::endl << boost::algorithm::join(this->desc, "\n") << std::endl;
        }
        return os.str();
    }

    //print door
    std::ostream& operator<<(std::ostream& os, const Door& rhs) {
        os << "- " << rhs.dir;
        if (!rhs.desc.empty()) {
            os << ". ";
            os << boost::algorithm::join(rhs.desc, "\n");
        } else {
            os << std::endl;
        }
        return os;
    }

    std::string Room::doorsToString() const {
        std::ostringstream os;
        os << "\nExits:\n";
        std::for_each(this->doors.begin(), this->doors.end(), [&os](const auto &d) {os << d;});
        return os.str();
    }

    //room state
    std::string Room::toString() const {
        std::ostringstream os;
        os << "\nCurrent Room\n";
        os << "------------\n";
        os << "Id: [" << this->id << "]\n";
        os << "Name: [" << this->name << "]\n";
        auto desc = boost::algorithm::join(this->desc, "\n");
        os << "Desc: [" << desc << "]\n";
        for (const auto &door : this->doors) {
            os << "Door:\n";
            os << "- Direction: [" << door.dir << "]\n";
            os << "- Destination: [" << door.leadsTo << "]\n";
            auto doorDesc = boost::algorithm::join(door.desc, "\n");
            os << "- Description: [" << doorDesc << "]\n";
        }
        for (const auto &extra : this->extras) {
            os << "Extra:\n";
            auto keywords = boost::algorithm::join(extra.getExtraKeywords(), ", ");
            os << "- Keywords: [" << keywords << "]" << std::endl;
            auto extraDesc = boost::algorithm::join(extra.getExtraDescriptions(), "\n");
            os << "- Description: [" << extraDesc << "]" << std::endl;
        }
        return os.str();
    }

    //print room
    std::ostream& operator<<(std::ostream& os, const Room& room) {
        os << std::endl << room.name << std::endl;
        os << std::string(room.name.length(), '-') << std::endl;

        if(!room.desc.empty()) {
            os << boost::algorithm::join(room.desc, "\n") << std::endl;
        }

        if(!room.doors.empty()) {
            os << "Exits:\n";
            std::for_each(room.doors.begin(), room.doors.end(), [&os](const auto &d) {os << d;});
        }

        if(!room.objects.empty()) {
            os << "Objects:\n";
            for (const auto &obj : room.objects) {
                os << "- " << obj.getShortDescription() << std::endl;
            }
        }

        if(!room.npcs.empty()) {
            os << "NPCS:\n";
            for (const auto &npc : room.npcs) {
                os << "- " << npc.getShortDescription() << std::endl;
            }
        }

        return os;
    }

}
