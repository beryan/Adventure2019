//
// Created by arehal on 1/17/19.
//

#ifndef WEBSOCKETNETWORKING_WORLD_H
#define WEBSOCKETNETWORKING_WORLD_H

#include <map>
#include <vector>
#include "Area.h"

using model::Area;

namespace model{
    class World{
    private:
        std::vector<Area> areas;

        friend std::ostream& operator<<(std::ostream& os, const World& rhs);

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

#endif //WEBSOCKETNETWORKING_WORLD_H
