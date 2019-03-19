/*
* Area.h
*
* Class Description: A class designed to represent an area.
*
* Created on: January 20, 2019
* Author: Brittany Ryan, 301217765
*/

#ifndef AREA_H
#define AREA_H

#include <string>
#include <vector>
#include "Room.h"
#include "Reset.h"

using model::Room;
using model::Reset;

namespace model {

    class Area {

    private:
        std::string name;
        std::vector<Room> rooms;
        std::vector<NPC> npcs;
        std::vector<Object> objects;
        std::vector<Reset> resets;

        friend std::ostream& operator<<(std::ostream& os, const Area& rhs);

    public:
        //constructors
        Area();
        Area(std::string name);
        Area(std::string name, std::vector<Room> rooms);

        //getters and setters
        std::string getName() const;
        std::vector<Room>& getRooms();
        std::vector<NPC> getNpcs() const;
        std::vector<Object> getObjects() const;
        std::vector<Reset> getResets() const;
        void setName(const std::string &name);
        void setRooms(const std::vector<Room> &rooms);
        void setNpcs(const std::vector<NPC> &npcs);
        void setObjects(const std::vector<Object> &objects);
        void setResets(const std::vector<Reset> &resets);

        void addRoom(const Room &room);
        void addNpc(const NPC &npc);
        void addObject(const Object &object);
        void addReset(const Reset &reset);

        std::vector<Room>::iterator findRoomById(model::ID roomID);
        std::vector<NPC>::iterator findNpcById(model::ID npcID);
        std::vector<Object>::iterator findObjectById(model::ID objectID);

        bool npcExists(const model::ID &npcId);
        bool objectExists(const model::ID &objectId);

        void addNpcsToRooms();
        void addObjectsToRooms();

        bool operator==(const Area& area) const;
    };

	inline void from_json(const json &j, Area &a) {
		a.setName(j.at("name").get<std::string>());
	}

}

#endif //AREA_H
