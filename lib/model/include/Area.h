/*
* Area.h
*
* Class Description: A class designed to represent an area.
*
* Created on: January 20, 2019
* Author: Brittany Ryan, 301217765
*/

#ifndef WEBSOCKETNETWORKING_AREA_H
#define WEBSOCKETNETWORKING_AREA_H

#include <string>
#include <vector>
#include "NPC.h"
#include "Object.h"
#include "Room.h"

using model::NPC;
using model::Object;
using model::Room;

namespace model {

    class Area {

    private:
        std::string name;
        std::vector<NPC> npcs;
        std::vector<Object> objects;
        std::vector<Room> rooms;

      	friend std::ostream& operator<<(std::ostream& os, const Area& rhs);

    public:
      	//constructors
      	Area();
      	Area(std::string name);
        Area(std::string name, std::vector<NPC> npcs, std::vector<Object> objects, std::vector<Room> rooms);

        //getters and setters
        std::string getName();
        std::vector<NPC> getNpcs();
        std::vector<Object> getObjects();
        std::vector<Room> getRooms();
        void setName(std::string name);
        void setNpcs(std::vector<NPC> npcs);
        void setObjects(std::vector<Object> objects);
        void setRooms(std::vector<Room> rooms);

        void addNPC(NPC npc);
        void addObject(Object object);
        void addRoom(Room room);

    };
}

#endif
