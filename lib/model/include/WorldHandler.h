//
// Created by arehal on 1/18/19.
//

#ifndef WEBSOCKETNETWORKING_WORLDHANDLER_H
#define WEBSOCKETNETWORKING_WORLDHANDLER_H

#include "Player.h"
#include "World.h"
#include "json.hpp"

using json = nlohmann::json;

namespace model {
    class WorldHandler {
    public:
        WorldHandler();

        World
        getWorld() const;

        /**
         *  Finds room based on room id
         */
        Room&
        findRoom(const model::ID &roomId);

        /**
         *  Determines if you can move from a room in a specified direction
         */
        bool
        isValidDirection(const model::ID &roomId, const std::string &dir);

        /**
         *  Gets resulting room id when you move from a room in a specified direction
         */
        model::ID
        getDestination(const model::ID &roomId, const std::string &dir);

        /**
         *  Adds player id to room with room id
         */
        void
        addPlayer(const model::ID &roomId, const model::ID &playerId);

        /**
         *  Removes player id from room with room id
         */
        void
        removePlayer(const model::ID &roomId, const model::ID &playerId);

        /**
         *  Moves player id from room with sourceId to room with destinationId
         */
        void
        movePlayer(const model::ID &playerId, const model::ID &sourceId, const model::ID &destinationId);

        /**
         *  Removes item from room
         */
        void
        removeItem(Room &room, const Object &item);

        /**
         *  Adds item to room
         */
        void
        addItem(Room &room, const Object &item);

        /**
         *  Gets player ids in current and adjacent rooms given current room id
         */
        std::vector<model::ID>
        getNearbyPlayerIds(const model::ID &roomId);

    private:
        World world;
    };
}

#endif //WEBSOCKETNETWORKING_WORLDHANDLER_H
