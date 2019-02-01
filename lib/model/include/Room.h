/*
* Room.h
*
* Class Description: A class designed to represent a room.
*
* Created on: January 18, 2019
* Author: Brittany Ryan, 301217765
*/

#ifndef WEBSOCKETNETWORKING_ROOM_H
#define WEBSOCKETNETWORKING_ROOM_H

#include <string>
#include <vector>
#include "Door.h"
#include "NPC.h"
#include "Player.h"

using model::Door;
using model::NPC;
using model::Object;

namespace model {

    class Room {

    private:
		model::ID id;
        std::string name;
      	std::vector<std::string> desc;
      	std::vector<Door> doors;
        std::vector<NPC> npcs;
        std::vector<Object> objects;

      	friend std::ostream& operator<<(std::ostream& os, const Room& rhs);

    public:
      	//constructors
      	Room();
      	Room(model::ID id, std::string name, std::vector<std::string> desc, std::vector<Door> doors);
        Room(model::ID id, std::string name, std::vector<std::string> desc, std::vector<Door> doors, std::vector<NPC> npcs, std::vector<Object> objects);

        //getters and setters
		model::ID getId();
        std::string getName();
        std::vector<std::string> getDesc();
        std::vector<Door> getDoors();
        std::vector<NPC> getNpcs();
        std::vector<Object> getObjects();
        void setId(model::ID id);
        void setName(std::string name);
        void setDesc(std::vector<std::string> desc);
        void setDoors(std::vector<Door> doors);
        void setNpcs(std::vector<NPC> npcs);
        void setObjects(std::vector<Object> objects);

        void addNPC(NPC npc);
        void addObject(Object object);

        void createStub();

    };
}

#endif
