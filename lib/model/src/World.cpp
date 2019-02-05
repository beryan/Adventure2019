//
// Created by arehal on 1/17/19.
//

#include <iostream>

#include "World.h"

using model::World;
using model::Player;

namespace model {

    World::World()
      : areas({})
        { }

    World::World(std::vector<Area> areas)
      : areas(std::move(areas))
        { }

    //getters and setters
    std::vector<Area> World::getAreas() const {
      return areas;
    }

    void World::setAreas(std::vector<Area> areas) {
      this->areas = std::move(areas);
    }

    void World::addArea(Area area) {
      areas.push_back(std::move(area));
    }

    std::map<model::ID, Player> World::getUserMap(){
        return this->usersMap;
    }

    void World::insertUser(Player player){
        usersMap.insert(std::pair<model::ID,Player>(player.getId(), player));
    }

//    std::map<model::ID, Area> World::getAreaMap(){
//        return this->areasMap;
//    }

    void World::printUsers() {
        std::cout << "Users:" << std::endl;

        for (auto& [key, value]: usersMap) {
            std::cout << value.getId() << ". " <<  value.getUsername() << std::endl;
        }
    }

    //create temporary world to test before json support
    void World::createStub() {
        Room room1 = {1, "Starting area", {"Welcome to Adventure 2019! You are in the starting area."}};
        room1.addDoor({"east",8800, {"You hear the sounds of forest creatures"}});
        Room room2 = {8800, "Entrance to Mirkwood", {"You have entered the quasi-magical Elven forest of Mirkwood."}};
        room2.addDoor({"west",1, {"Back to the starting area"}});
        room2.addDoor({"south",1100, {"You see a clearing"}});
        Room room3 = {1100, "A dimly lit path", {"You tread through the deep, dark forest on a dimly lit path.","You've reached a dead end."}};
        room3.addDoor({"north",8800, {"The path to Mirkwood"}});
        Area area1 = Area();
        area1.addRoom(room1);
        area1.addRoom(room2);
        area1.addRoom(room3);
        this->addArea(area1);
    }
};
