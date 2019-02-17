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
        std::map<model::ID, Player> usersMap;
        //std::map<model::ID, Area> areasMap;

        friend std::ostream& operator<<(std::ostream& os, const World& rhs);

    public:
        World();

        World(std::vector<Area> areas);

        //getters and setters
        std::vector<Area>
        getAreas() const;

        void
        setAreas(std::vector<Area> areas);

        void
        addArea(Area area);

        void
        removePlayer(const model::ID &playerID, const model::ID &roomID);

        void
        addPlayer(const model::ID &playerID, const model::ID &roomID);

        std::map<model::ID, Player>
        getUserMap();

        /*
        std::map<model::ID, Character>
        getAreaMap();
         */

        void
        insertUser(Player player);

        void
        printUsers();

        void
        createStub();
    };
}

#endif //WEBSOCKETNETWORKING_WORLD_H
