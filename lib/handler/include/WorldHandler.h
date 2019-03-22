//
// Created by arehal on 1/18/19.
//

#ifndef WORLDHANDLER_H
#define WORLDHANDLER_H

#include "Player.h"
#include "World.h"
#include "ResetHandler.h"
#include "json.hpp"

using json = nlohmann::json;
using handler::ResetHandler;
using model::World;
using model::Room;

namespace handler {
    class WorldHandler {
    public:
        WorldHandler();

        World
        getWorld() const;

        /**
         *  Determines if room exists based on room id
         */
        bool
        roomExists(const model::ID &roomId);

        /**
         *  Finds room based on room id
         */
        Room&
        findRoom(const model::ID &roomId);

        /**
         *  Finds area based on room id
         */
        Area&
        findArea(const model::ID &roomId);

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
         *  Adds item to room
         */
        void
        addItem(const model::ID &roomId, const Object &item);

        /**
         *  Removes item from room by id
         */
        void
        removeItem(const model::ID &roomId, const model::ID &objectId);

        /**
         *  Determines if you can give to a player
         */
        bool
        canGive(const model::ID &roomId, const model::ID &playerId);

        /**
         *  Gets player ids in current and adjacent rooms given current room id
         */
        std::vector<model::ID>
        getNearbyPlayerIds(const model::ID &roomId);

        bool
        createArea(const std::string &parameters);

        bool
        createRoom(const std::string &parameters);

        bool
        createObject(const std::string &parameters);

        bool
        createNpc(const std::string &parameters);

        bool
        createObjectReset(const model::ID &roomId, const std::string &parameters);

        bool
        createNpcReset(const model::ID &roomId, const std::string &parameters);

        bool
        editArea(const model::ID &roomId, const std::string &parameters);

        bool
        editRoom(const model::ID &roomId, const std::string &parameters);

        bool
        editObject(const model::ID &roomId, const std::string &parameters);

        bool
        editNpc(const model::ID &roomId, const std::string &parameters);

        void
        resetAreas();

    private:
        World world;
        ResetHandler resetHandler;

        bool
        isNum(const std::string &str);
    };
}

#endif //WORLDHANDLER_H
