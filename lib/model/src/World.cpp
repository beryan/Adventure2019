//
// Created by arehal on 1/17/19.
//

#include <iostream>

#include "World.h"

using model::World;

namespace model {

    World::World()
        : areas({})
          { }


    World::World(std::vector<Area> areas)
        : areas(std::move(areas))
          { }


    //getters and setters
    std::vector<Area>
    World::getAreas() const {
        return areas;
    }


    void
    World::setAreas(std::vector<Area> areas) {
        this->areas = std::move(areas);
    }


    void
    World::addArea(Area area) {
        areas.push_back(std::move(area));
    }


    void
    World::removePlayer(const model::ID &playerID, const model::ID &roomID) {
        for (auto &area : this->areas) {
            if (area.removePlayer(playerID, roomID)) {
                return;
            }
        }
    }


    void
    World::addPlayer(const model::ID &playerID, const model::ID &roomID) {
        for (auto &area : this->areas) {
            if (area.addPlayer(playerID, roomID)) {
                return;
            }
        }
    }


    //create temporary world to test before json support
    void
    World::createStub() {
        Room room1 = {1, "Starting area", {"You are in the starting area."}};
        room1.addDoor({"east",8800, {"You hear the sounds of forest creatures"}});
        Room room2 = {8800, "Entrance to Mirkwood", {"You have entered the quasi-magical Elven forest of Mirkwood."}};
        room2.addDoor({"west",1, {"Back to the starting area"}});
        room2.addDoor({"south",1100, {"You see a clearing"}});
        Room room3 = {1100, "A dimly lit path", {"You tread through the deep, dark forest on a dimly lit path.","You've reached a dead end."}};
        room3.addDoor({"north",8800, {"The path to Mirkwood"}});
        Area area1 = Area("area1");
        area1.addRoom(room1);
        area1.addRoom(room2);
        area1.addRoom(room3);
        this->addArea(area1);
    }

    //print world
    std::ostream&
    operator<<(std::ostream& os, const World& rhs) {
        os << "\nWorld state:\n";
        os << "------------\n";
        for (const auto &area : rhs.areas) {
            os << area;
        }

        return os;
    }
}
