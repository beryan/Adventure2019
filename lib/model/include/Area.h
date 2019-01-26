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
#include "NPC.h"
#include "Room.h"

namespace model {

	/**
    *  @class Area
    *
    *  @brief A class describing an area.
    *
    *  The Area class represents a collection of Room objects.
    */

    class Area {
	public:
		Area();

		Area(std::string name);

		Area(std::string name, std::vector<NPC> NPCObjects, std::vector<Object> items, std::vector<Room> rooms);

		std::string getName() const;

		void setName(std::string name);

		std::vector<NPC> getNPCObjects() const;

		void setNPCObjects(std::vector<NPC> NPCObjects);

		void addNPCObject(NPC NPCObjectToAdd);

		std::vector<Object> getObjects() const;

		void setObjects(std::vector<Object> items);

		void addObject(Object objectToAdd);

		std::vector<Room> getRooms() const;

		void setRooms(std::vector<Room> rooms);

		void addRoom(Room roomToAdd);

		friend std::ostream& operator<<(std::ostream& out, const Area& Area);

	private:
        std::string name;

        std::vector<NPC> NPCObjects;

        std::vector<Object> objects;

        std::vector<Room> rooms;
    };
}

#endif // AREA_H