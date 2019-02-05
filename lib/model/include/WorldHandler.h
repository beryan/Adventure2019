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

        /**
         *  Finds room based on room ID
         */
        Room
        findRoom(model::ID roomID);

        /**
         *  Determines if you can move from a room in a specified direction
         */
        bool
        isValidDirection(model::ID roomID, std::string dir);

        /**
         *  Gets resulting room id when you move from a room in a specified direction
         */
        model::ID
        getDestination(model::ID roomID, std::string dir);

        /**
         *  Removes player ID from playersInRoom vector of current room and adds it to playerInRoom vector of destination room
         */
        void
        movePlayer(model::ID playerID, model::ID sourceID, model::ID destinationID);

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
