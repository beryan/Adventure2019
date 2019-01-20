//
// Created by arehal on 1/18/19.
//

#ifndef WEBSOCKETNETWORKING_WORLDHANDLER_H
#define WEBSOCKETNETWORKING_WORLDHANDLER_H

#include "Player.h"
#include "World.h"
#include "json.hpp"

using json = nlohmann::json;

namespace model{
    class WorldHandler{
    public:
        WorldHandler();

        void addUser(Player player);
        void removeUser(Player player);

//        void addArea(Area area);
//        void removeArea(Area area);

        void parseJSON();

        void createUsersFromJSON(json user);

    private:
        World world;
    };
}

#endif //WEBSOCKETNETWORKING_WORLDHANDLER_H
