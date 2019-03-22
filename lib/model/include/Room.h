/*
* Room.h
*
* Class Description: A class designed to represent a room.
*
* Created on: January 18, 2019
* Author: Brittany Ryan, 301217765
*/

#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <vector>
#include "NPC.h"
#include "Player.h"
#include "ExtraObjectInfo.h"

using model::NPC;
using model::Object;
using model::ExtraObjectInfo;

namespace model {

    struct Door {
        std::string dir; //direction
        model::ID leadsTo; //id of room door leads to
        std::vector<std::string> desc;
        std::vector<std::string> keywords;
    };

    class Room {

    private:
        model::ID id;
        std::string name;
        std::vector<std::string> desc;
        std::vector<Door> doors;
        std::vector<NPC> npcs;
        std::vector<Object> objects;
        std::vector<model::ID> playersInRoom;
        std::vector<ExtraObjectInfo> extras;

        friend std::ostream& operator<<(std::ostream& os, const Door& rhs);
        friend std::ostream& operator<<(std::ostream& os, const Room& rhs);

    public:
        //constructors
        Room();
        Room(model::ID id, std::string name, std::vector<std::string> desc);
        Room(model::ID id, std::string name, std::vector<std::string> desc, std::vector<Door> doors, std::vector<NPC> npcs, std::vector<Object> objects);

        //getters and setters
        model::ID getId() const;
        std::string getName() const;
        std::vector<std::string> getDesc() const;
        std::vector<Door> getDoors() const;
        std::vector<NPC>& getNpcs();
        NPC& getNpc(const model::ID &id);
        std::vector<Object> getObjects() const;
        std::vector<model::ID> getPlayersInRoom() const;
        std::vector<ExtraObjectInfo> getExtras() const;
        void setId(const model::ID &id);
        void setName(const std::string &name);
        void setDesc(const std::vector<std::string> &desc);
        void setDoors(const std::vector<Door> &doors);
        void setNpcs(const std::vector<NPC> &npcs);
        void setObjects(const std::vector<Object> &objects);
        void setPlayersInRoom(const std::vector<model::ID> &playersInRoom);

        void addDoor(const Door &door);
        void addNPC(const NPC &npc);
        void addObject(const Object &object);
        void addPlayerToRoom(const model::ID &playerId);
        void addExtra(const ExtraObjectInfo &extra);

        void removeObject(const model::ID &objectId);
        void removePlayerFromRoom(const model::ID &playerId);

        bool isValidDirection(const std::string &dir) const;
        model::ID getDestination(const std::string &dir) const;
        std::vector<model::ID> getNearbyRoomIds() const;

        int countNpcById(const model::ID& npcID) const;
        int countObjectById(const model::ID& objectID) const;

        std::string descToString() const;
        std::string doorsToString() const;

        bool operator==(const Room& room) const;

    };

    inline void from_json(const json &j, Door &d) {
        j.at("dir").get_to(d.dir);
        j.at("to").get_to(d.leadsTo);
        j.at("desc").get_to(d.desc);
        j.at("keywords").get_to(d.keywords);
    }

    inline void from_json(const json &j, Room &r) {
        r.setId(j.at("id").get<model::ID>());
        r.setName(j.at("name").get<std::string>());
        r.setDesc(j.at("desc").get<std::vector<std::string>>());
        r.setDoors(j.at("doors").get<std::vector<Door>>());
    }
}

#endif //ROOM_H
