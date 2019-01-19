/*
* Door.cpp
*
* Class Description: A class designed to represent exits from a room.
*
* Created on: January 17, 2019
* Author: Brittany Ryan, 301217765
*/

#include "Door.h"
#include <iostream>

using model::Door;

namespace model {

    //constructors
    Door::Door() {
    	dir = "";
    	desc = {};
    	keywords = {};
    	to = -1;
    }

    Door::Door(std::string d, int t) {
    	dir = d;
    	desc = {};
    	keywords = {};
    	to = t;
    }

    Door::Door(std::string direction, std::vector<std::string> description, std::vector<std::string> k, int t) {
    	dir = direction;
    	desc = description;
    	keywords = k;
    	to = t;
    }

    //getters and setters
    std::string Door::getDir() {
    	return dir;
    }

    std::vector<std::string> Door::getDesc() {
    	return desc;
    }

    std::vector<std::string> Door::getKeywords() {
    	return keywords;
    }

    int Door::getTo() {
    	return to;
    }

    void Door::setDir(std::string d) {
    	dir = d;
    }

    void Door::setDesc(std::vector<std::string> d) {
    	desc = d;
    }

    void Door::setKeywords(std::vector<std::string> k) {
    	keywords = k;
    }

    void Door::setTo(int t) {
    	to = t;
    }

    //print object
    std::ostream& operator<<(std::ostream& os, const Door& rhs) {
    	os << "Direction: " << rhs.dir << std::endl;

      if (rhs.desc.size() != 0) {
      	for (std::string s : rhs.desc) {
      		os << s << std::endl;
      	}
      }

    	return os;
    }

}
