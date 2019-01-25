/*
* Door.cpp
*
* Class Description: A class designed idOfRoomDoorLeadsTo represent exits from a room.
*
* Created on: January 17, 2019
* Author: Brittany Ryan, 301217765
*/

#include "Door.h"
#include <iostream>


namespace model {

    Door::Door() :
        direction({}),
        description({}),
        keywords({}),
        idOfRoomDoorLeadsTo(Door::DEFAULT_ID_OF_ROOM_DOOR_LEADS_TO)
        {}

    Door::Door(std::string direction, int idOfRoomDoorLeadsTo) :
        direction(std::move(direction)),
        description({}),
        keywords({}),
        idOfRoomDoorLeadsTo(idOfRoomDoorLeadsTo)
        { }

    Door::Door(
        std::string direction,
        std::vector<std::string> description,
        std::vector<std::string> keywords,
        int idOfRoomDoorLeadsTo
    ) :
        direction(std::move(direction)),
        description(std::move(description)),
        keywords(std::move(keywords)),
        idOfRoomDoorLeadsTo(idOfRoomDoorLeadsTo)
        { }

    std::string Door::getDirection() const {
    	return this->direction;
    }

    std::vector<std::string> Door::getDescriptions() const {
    	return this->description;
    }

    std::vector<std::string> Door::getKeywords() const {
    	return this->keywords;
    }

    int Door::getIdOfRoomDoorLeadsTo() const {
    	return this->idOfRoomDoorLeadsTo;
    }

    void Door::setDirection(std::string direction) {
    	this->direction = std::move(direction);
    }

    void Door::setDescriptions(std::vector<std::string> descriptions) {
    	this->description = std::move(descriptions);
    }

    void Door::setKeywords(std::vector<std::string> keywords) {
    	this->keywords = std::move(keywords);
    }

    void Door::setIdOfRoomDoorLeadsTo(int idOfRoomDoorLeadsTo) {
    	this->idOfRoomDoorLeadsTo = idOfRoomDoorLeadsTo;
    }

    std::ostream& operator<<(std::ostream& out, const Door& door) {
        out << "Direction: " << door.direction << std::endl;

        if (!door.description.empty()) {
            for (const std::string &description : door.description) {
                out << description << std::endl;
            }
        }

    	return out;
    }

}