//
// Created by arehal on 1/17/19.
//

#ifndef WORLD_H
#define WORLD_H

#include <map>
#include <vector>
#include "Area.h"

using model::Area;

namespace model{
    class World{
    private:
        std::vector<Area> areas;

    public:
        //constructors
        World();
        World(std::vector<Area> areas);

        //getters and setters
        std::vector<Area>& getAreas();
        void setAreas(const std::vector<Area> &areas);

        void addArea(const Area &area);

        void createStub();
    };
}

#endif //WORLD_H
