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
#include "Room.h"

using model::Room;

namespace model {

    class Area {

    private:
        std::string name;
        std::vector<Room> rooms;

        friend std::ostream& operator<<(std::ostream& os, const Area& rhs);

    public:
        //constructors
        Area();

        Area(std::string name);

        Area(std::string name, std::vector<Room> rooms);

        //getters and setters
        std::string
        getName() const;

        std::vector<Room>
        getRooms() const;

        void
        setName(std::string name);

        void
        setRooms(std::vector<Room> rooms);

        void
        addRoom(Room room);

        bool
        removePlayer(const model::ID &playerID, const model::ID &roomID);

        bool
        addPlayer(const model::ID &playerID, const model::ID &roomID);

        bool
        operator==(const Area& area) const;
    };
}

#endif
