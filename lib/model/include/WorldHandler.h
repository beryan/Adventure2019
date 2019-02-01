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

        World
        getWorld() const;

        /**
         *  Finds room based on room ID
         */
        Room
        findRoom(const model::ID &roomID);

        /**
         *  Determines if you can move from a room in a specified direction
         */
        bool
        isValidDirection(const model::ID &roomID, const std::string &dir);

        /**
         *  Gets resulting room id when you move from a room in a specified direction
         */
        model::ID
        getDestination(const model::ID &roomID, const std::string &dir);

        void
        addPlayer(const model::ID &playerID, const model::ID &roomID);

        void
        removePlayer(const model::ID &playerID, const model::ID &roomID);

        /**
         *  Removes player ID from playersInRoom vector of current room and adds it to playerInRoom vector of destination room
         */
        void
        movePlayer(const model::ID &playerID, const model::ID &sourceID, const model::ID &destinationID);

        /**
         *  Gets player IDs in current and adjacent rooms given current room ID
         */
        std::vector<model::ID>
        getNearbyPlayerIds(const model::ID &roomId);

        void addUser(Player player);
        void removeUser(Player player);

//        void addArea(Area area);
//        void removeArea(Area area);

        void parseJSON();

        void parseUserData();

    private:
        World world;
    };
}

#endif //WEBSOCKETNETWORKING_WORLDHANDLER_H
