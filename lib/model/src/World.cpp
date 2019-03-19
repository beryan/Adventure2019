//
// Created by arehal on 1/17/19.
//

#include <iostream>
#include "World.h"

using model::World;

namespace model {

    //constructors
    World::World()
        : areas({})
          { }

    World::World(std::vector<Area> areas)
        : areas(std::move(areas))
          { }

    //getters and setters
    std::vector<Area>& World::getAreas() {
        return areas;
    }

    void World::setAreas(const std::vector<Area> &areas) {
        this->areas = areas;
    }

    void World::addArea(const Area &area) {
        this->areas.push_back(area);
    }

    //create temporary world to test before json support
    void World::createStub() {
        Room room1 = {1000, "Starting room", {"You are in the starting area. You look around and see an elven sword and cloak.","There is also a mysterious key."}};
        room1.addDoor({"east",1001, {"You hear the sounds of forest creatures in this direction."}});
        Room room2 = {1001, "Entrance to Mirkwood", {"You have entered the quasi-magical Elven forest of Mirkwood.","There is a sign tacked to a tree nearby."}};
        room2.addDoor({"west",1000, {"The path back to the starting area."}});
        room2.addDoor({"south",1003});
        room2.addExtra({{"sign"},{"This area serves as a connector for many areas with an Elven or Faerie",
                                 "flavour. Tread carefully, and remember to keep track of where you have been.",
                                 "--Unknown"}});
        Room room3 = {1003, "A dimly lit path", {"You tread through the deep, dark forest on a dimly lit path.","You've reached a dead end."}};
        room3.addDoor({"north",1001});
        room3.addDoor({"west",8803,{"Under construction."}});
        Area area1 = Area("Starting area");
        area1.addRoom(room1);
        area1.addRoom(room2);
        area1.addRoom(room3);
        area1.addObject({1000, "a fine elven sword", {"A fine Elven sword glimmers softly here."}, {"elven","sword"}, Slot::Weapon});
        area1.addObject({1001, "a plain cloak", {"A plain, but practical, cloak lies here."}, {"cloak"}, Slot::Back});
        area1.addObject({1002, "a key", {"A key to a cell door is here."}, {"key"}, Slot::Misc});
        area1.addObject({1003, "an orcish scimitar", {"An Orcish scimitar has been left here."}, {"scimitar"}, Slot::Weapon});
        area1.addNpc({1000,
                      {"thorin","oakenshield"},
                      {"This Dwarf is descended from noble blood. He and his fellows were waylaid",
                       "by the Wood Elves while journeying to avenge their brethren against the",
                       "dragon Smaug."},
                      "Thorin Oakenshield",
                      {"Thorin Oakenshield politely bows before you."}});
        area1.addReset({"object",1000,-1,1000,-1});
        area1.addReset({"object",1001,-1,1000,-1});
        area1.addReset({"object",1002,-1,1000,-1});
        area1.addReset({"npc",1000,1,1001,-1});
        area1.addReset({"object",1003,-1,1003,-1});
        this->addArea(area1);
    }

    void World::reset() {
        for (auto &area : this->areas) {
            area.addNpcsToRooms();
            area.addObjectsToRooms();
        }
    }

    //print world
    std::ostream& operator<<(std::ostream& os, const World& rhs) {
        int count = 1;
        os << "\nWorld state:\n";
        os << "------------\n";
        for (const auto &area : rhs.areas) {
            os << count << ". " << area;
            count++;
        }

        return os;
    }
}