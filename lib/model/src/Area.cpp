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

    std::vector<Room> Area::getRooms() const {
      return rooms;
    }

    void Area::setName(std::string name) {
      this->name = std::move(name);
    }

    void Area::setRooms(std::vector<Room> rooms) {
      this->rooms = std::move(rooms);
    }

    void Area::addRoom(Room room) {
      rooms.push_back(std::move(room));
    }

    //print object
    std::ostream& operator<<(std::ostream& os, const Area& rhs) {
      os << "You are in " << rhs.name << std::endl;

    	return os;
    }

}
