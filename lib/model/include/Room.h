//
// Created by Brittany Ryan on 2019-01-18.
//

#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <vector>
#include "NPC.h"
#include "Player.h"
#include "ExtraInfo.h"

using model::NPC;
using model::Object;
using model::ExtraInfo;

namespace model {
    /**
    *  @struct Door
    *
    *  @brief A struct designed to represent a door.
    *
    *  The Door class contains all door information needed for the game
    *  including direction, destination, and description
    */
    struct Door {
        std::string dir; //direction
        model::ID leadsTo; //id of room door leads to
        std::vector<std::string> desc;
        std::vector<std::string> keywords;
    };

    /**
    *  @class Room
    *
    *  @brief A class designed to represent a room.
    *
    *  The Room class contains all room information needed for the game
    *  including id, name, description, doors, npcs, objects, players, and extras
    */
    class Room {

    private:
        model::ID id;
        std::string name;
        std::vector<std::string> desc;
        std::vector<Door> doors;
        std::vector<NPC> npcs;
        std::vector<Object> objects;
        std::vector<model::ID> playersInRoom;
        std::vector<ExtraInfo> extras;

        friend std::ostream& operator<<(std::ostream& os, const Door& rhs);
        friend std::ostream& operator<<(std::ostream& os, const Room& rhs);

    public:
        //constructors
        Room();
        Room(model::ID id, std::string name);
        Room(model::ID id, std::string name, std::vector<std::string> desc);
        Room(model::ID id, std::string name, std::vector<std::string> desc, std::vector<Door> doors, std::vector<NPC> npcs, std::vector<Object> objects);

        //getters and setters
        model::ID getId() const;
        std::string getName() const;
        std::vector<std::string> getDesc() const;
        std::vector<Door> getDoors() const;
        std::vector<NPC>& getNpcs();
        NPC& getNpcByUniqueId(const model::ID &id);
        NPC& getNpcByKeyword(const std::string &param);
        std::vector<Object> getObjects() const;
        std::vector<model::ID> getPlayersInRoom() const;
        std::vector<ExtraInfo> getExtras() const;
        void setId(const model::ID &id);
        void setName(const std::string &name);
        void setDesc(const std::vector<std::string> &desc);
        void setDoors(const std::vector<Door> &doors);
        void setNpcs(const std::vector<NPC> &npcs);
        void setObjects(const std::vector<Object> &objects);
        void setPlayersInRoom(const std::vector<model::ID> &playersInRoom);
        void setExtras(const std::vector<ExtraInfo> &extras);

        void addDoor(const Door &door);
        void addNPC(const NPC &npc);
        void addObject(const Object &object);
        void addPlayerToRoom(const model::ID &playerId);
        void addExtra(const ExtraInfo &extra);

        void removeObject(const model::ID &objectId);
        void removePlayerFromRoom(const model::ID &playerId);

        bool isValidDirection(const std::string &dir) const;
        model::ID getDestination(const std::string &dir) const;
        std::vector<model::ID> getNearbyRoomIds() const;

        int countNpcById(const model::ID& npcID) const;
        int countObjectById(const model::ID& objectID) const;

        std::string descToString() const;
        std::string doorsToString() const;
        std::string toString() const;

        bool operator==(const Room& room) const;

    };

    inline void from_json(const Json &json, Door &door) {
        json.at("dir").get_to(door.dir);
        json.at("to").get_to(door.leadsTo);
        json.at("desc").get_to(door.desc);
        json.at("keywords").get_to(door.keywords);
    }

    inline void to_json(Json &json, const Door &door) {
        json = {
                {"dir", door.dir},
                {"to", door.leadsTo},
                {"desc", door.desc},
                {"keywords", door.keywords}
        };
    }

    inline void from_json(const Json &json, Room &room) {
        room.setId(json.at("id").get<model::ID>());
        room.setName(json.at("name").get<std::string>());
        room.setDesc(json.at("desc").get<std::vector<std::string>>());
        room.setDoors(json.at("doors").get<std::vector<Door>>());
        room.setExtras((json.at("extended_descriptions").get<std::vector<ExtraInfo>>()));
    }

    inline void to_json(Json &json, const Room &room) {
        json = {
                {"id", room.getId()},
                {"name", room.getName()},
                {"desc", room.getDesc()},
                {"doors", room.getDoors()},
                {"extended_descriptions", room.getExtras()}
        };
    }

}

#endif //ROOM_H
