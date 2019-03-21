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
        Room room1 = {8800, "Starting room", {"You are in the starting area. You look around and see an elven sword and cloak.","There is also a mysterious key."}};
        room1.addDoor({"east",8801, {"You hear the sounds of forest creatures in this direction."}});
        room1.addObject({8811, "a fine elven sword", {"A fine Elven sword glimmers softly here."}, {"elven","sword"}, Slot::Weapon});
        room1.addObject({8802, "a plain cloak", {"A plain, but practical, cloak lies here."}, {"cloak"}, Slot::Back});
        room1.addObject({8812, "a key", {"A key to a cell door is here."}, {"key"}, Slot::Misc});
        Room room2 = {8801, "Entrance to Mirkwood", {"You have entered the quasi-magical Elven forest of Mirkwood.","There is a sign tacked to a tree nearby."}};
        room2.addDoor({"west",8800, {"The path back to the starting area."}});
        room2.addDoor({"south",1100});
        room2.addObject({8812, "a key", {"A key to a cell door is here."}, {"key"}, Slot::Misc});
        room2.addNPC({8811,
                      {"thorin","oakenshield"},
                      {"This Dwarf is descended from noble blood. He and his fellows were waylaid",
                       "by the Wood Elves while journeying to avenge their brethren against the",
                       "dragon Smaug."},
                      "Thorin Oakenshield",
                      {"Thorin Oakenshield politely bows before you."}});
        room2.addExtra({{"sign"},{"This area serves as a connector for many areas with an Elven or Faerie",
                                 "flavour. Tread carefully, and remember to keep track of where you have been.",
                                 "--Uknown"}});
        Room room3 = {1100, "A dimly lit path", {"You tread through the deep, dark forest on a dimly lit path.","You've reached a dead end."}};
        room3.addDoor({"north",8801});
        room3.addObject({8806, "an orcish scimitar", {"An Orcish scimitar has been left here."}, {"scimitar"}, Slot::Weapon});
        Area area1 = Area("Starting area");
        area1.addRoom(room1);
        area1.addRoom(room2);
        area1.addRoom(room3);
        this->addArea(area1);
    }

    //print world
    std::ostream& operator<<(std::ostream& os, const World& rhs) {
        os << "\nWorld state:\n";
        os << "------------\n";
        for (const auto &area : rhs.areas) {
            os << area;
        }

        return os;
    }
}