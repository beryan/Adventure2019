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

using model::Door;

namespace model {

    class Room {

    private:
      	int id;
        std::string name;
      	std::vector<std::string> desc;
      	std::vector<Door> doors;

      	friend std::ostream& operator<<(std::ostream& os, const Room& rhs);

    public:
      	//constructors
      	Room();
      	Room(int id, std::string name, std::vector<std::string> desc, std::vector<Door> doors);

        //getters and setters
        int getId();
        std::string getName();
        std::vector<std::string> getDesc();
        std::vector<Door> getDoors();
        void setId(int id);
        void setName(std::string name);
        void setDesc(std::vector<std::string> desc);
        void setDoors(std::vector<Door> doors);

        void createStub();

    };
}

#endif
