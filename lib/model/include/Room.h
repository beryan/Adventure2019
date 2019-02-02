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
#include "NPC.h"
#include "Object.h"

using model::NPC;
using model::Object;

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

      	friend std::ostream& operator<<(std::ostream& os, const Room& rhs);

    public:
      	//constructors
      	Room();
      	Room(model::ID id, std::string name, std::vector<std::string> desc, std::vector<Door> doors);
        Room(model::ID id, std::string name, std::vector<std::string> desc, std::vector<Door> doors, std::vector<NPC> npcs, std::vector<Object> objects);

        //getters and setters
		    model::ID getId() const;
        std::string getName() const;
        std::vector<std::string> getDesc() const;
        std::vector<Door> getDoors() const;
        std::vector<NPC> getNpcs() const;
        std::vector<Object> getObjects() const;
        std::vector<model::ID> getPlayersInRoom() const;
        void setId(model::ID id);
        void setName(std::string name);
        void setDesc(std::vector<std::string> desc);
        void setDoors(std::vector<Door> doors);
        void setNpcs(std::vector<NPC> npcs);
        void setObjects(std::vector<Object> objects);
        void setPlayersInRoom(std::vector<model::ID> playersInRoom);

        void addDoor(Door door);
        void addNPC(NPC npc);
        void addObject(Object object);
        void addPlayerToRoom(model::ID playerId);
        void removePlayerFromRoom(model::ID playerId);

        void createStub();

    };
}

#endif
