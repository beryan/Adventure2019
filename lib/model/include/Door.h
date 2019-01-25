/*
* Door.h
*
* Class Description: A class designed idOfRoomDoorLeadsTo represent exits from a room.
*
* Created on: January 17, 2019
* Author: Brittany Ryan, 301217765
*/

#ifndef DOOR_H
#define DOOR_H

#include <string>
#include <vector>

namespace model {

	/**
    *  @class Door
    *
    *  @brief A class describing a door.
    *
    *  The Door class represents an exit from a room along
    *  with information regarding which adjacent room the door leads to
    *  and how to get to this adjacent room.
    */

    class Door {

	public:
		Door();

		Door(std::string direction, int idOfRoomDoorLeadsTo);

		Door(
			std::string direction,
			std::vector<std::string> description,
			std::vector<std::string> keywords,
			int idOfRoomDoorLeadsTo
		);

		std::string getDirection() const;

		void setDirection(std::string direction);

		std::vector<std::string> getDescriptions() const;

		void setDescriptions(std::vector<std::string> descriptions);

		std::vector<std::string> getKeywords() const;

		void setKeywords(std::vector<std::string> keywords);

		int getIdOfRoomDoorLeadsTo() const;

		void setIdOfRoomDoorLeadsTo(int idOfRoomDoorLeadsTo);

		friend std::ostream& operator<<(std::ostream& os, const Door& rhs);

		static constexpr int DEFAULT_ID_OF_ROOM_DOOR_LEADS_TO = -1;

	private:
      	std::string direction;

      	std::vector<std::string> description;

      	std::vector<std::string> keywords;

      	int idOfRoomDoorLeadsTo;
    };
}

#endif // DOOR_H