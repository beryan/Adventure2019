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
}