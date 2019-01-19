/*
* Room.cpp
*
* Class Description: A class designed to represent a room.
*
* Created on: January 18, 2019
* Author: Brittany Ryan, 301217765
*/

#include "Room.h"
#include <iostream>

using model::Room;

namespace model {

    //constructors
    Room::Room() {
      id = -1;
      name = "";
      desc = {};
      doors = {};
    }

    Room::Room(int id, std::string name, std::vector<std::string> desc, std::vector<Door> doors) {
      this->id = id;
      this->name = name;
      this->desc = desc;
      this->doors = doors;
    }

    //getters and setters
    int Room::getId() {
      return id;
    }

    std::string Room::getName() {
      return name;
    }

    std::vector<std::string> Room::getDesc() {
      return desc;
    }

    std::vector<Door> Room::getDoors() {
      return doors;
    }

    void Room::setId(int id) {
      this->id = id;
    }

    void Room::setName(std::string name) {
      this->name = name;
    }

    void Room::setDesc(std::vector<std::string> desc) {
      this->desc = desc;
    }

    void Room::setDoors(std::vector<Door> doors) {
      this->doors = doors;
    }

    //create temporary room to showcase functionality based on provided mirkwood file
    void Room::createStub() {
      this->id = 8800;
      this->name = "Entrance to Mirkwood";
      this->desc = {"You have entered the quasi-magical Elven forest of Mirkwood, populated by",
      "Sylvan Elves and their mysterious allies. Strangers are frowned upon here,",
      "so be on your guard. It is rumored that many different races of Elves",
      "actually inhabit this forest, in various parts and sometimes they are",
      "carefully hidden, away from the prying eyes of strangers."};
      Door stubDoor1 = Door("north", 8801);
      Door stubDoor2 = Door("south", {"You see an old archway.", "It is covered in spider webs."}, {"archway","webs"}, 8855);
      this->doors = {stubDoor1, stubDoor2};
    }

    //print object
    std::ostream& operator<<(std::ostream& os, const Room& rhs) {
      os << "\n*****" << rhs.name << "*****\n";

      if (rhs.desc.size() != 0) {
        for (std::string s : rhs.desc) {
      		os << s << std::endl;
      	}
      }

      os << "\n***Exits***\n";
    	for (Door d : rhs.doors) {
    		os << d;
    	}

    	return os;
    }

}
