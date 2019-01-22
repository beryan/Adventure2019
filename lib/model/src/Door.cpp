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
    Door::Door()
      : dir(""),
        desc({}),
        keywords({}),
        to(-1)
        { }

    Door::Door(std::string dir, int to)
    	: dir(std::move(dir)),
        desc({}),
        keywords({}),
        to(to)
        { }

    Door::Door(std::string dir, std::vector<std::string> desc, std::vector<std::string> keywords, int to)
      : dir(std::move(dir)),
        desc(std::move(desc)),
        keywords(std::move(keywords)),
        to(to)
        { }

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

    void Door::setDir(std::string dir) {
    	this->dir = std::move(dir);
    }

    void Door::setDesc(std::vector<std::string> desc) {
    	this->desc = std::move(desc);
    }

    void Door::setKeywords(std::vector<std::string> keywords) {
    	this->keywords = std::move(keywords);
    }

    void Door::setTo(int to) {
    	this->to = to;
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
