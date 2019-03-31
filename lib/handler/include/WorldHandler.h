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
    /**
     *  @class WorldHandler
     *
     *  @brief A class for managing game world state
     *
     *  This class contains methods to create, modify, and retrieve world state information
     */
    class WorldHandler {
    public:
        WorldHandler();

        World getWorld() const;

        void setWorld(const World &world);

        /**
         *  Determines if room exists based on room id
         */
        bool roomExists(const model::ID &roomId);

        /**
         *  Finds room based on room id
         */
        Room& findRoom(const model::ID &roomId);

        /**
         *  Finds area based on room id
         */
        Area& findArea(const model::ID &roomId);

        /**
         *  Determines if you can move from a room in a specified direction
         */
        bool isValidDirection(const model::ID &roomId, const std::string &dir);

        /**
         *  Gets resulting room id when you move from a room in a specified direction
         */
        model::ID getDestination(const model::ID &roomId, const std::string &dir);

        /**
         *  Adds player id to room with room id
         */
        void addPlayer(const model::ID &roomId, const model::ID &playerId);

        /**
         *  Removes player id from room with room id
         */
        void removePlayer(const model::ID &roomId, const model::ID &playerId);

        /**
         *  Moves player id from room with sourceId to room with destinationId
         */
        void movePlayer(const model::ID &playerId, const model::ID &sourceId, const model::ID &destinationId);

        /**
         *  Adds item to room
         */
        void addItem(const model::ID &roomId, const Object &item);

        /**
         *  Removes item from room by id
         */
        void removeItem(const model::ID &roomId, const model::ID &objectId);

        /**
         *  Determines if you can give to a player
         */
        bool canGive(const model::ID &roomId, const model::ID &playerId);

        /**
         *  Gets player ids in current and adjacent rooms given current room id
         */
        std::vector<model::ID> getNearbyPlayerIds(const model::ID &roomId);

        /**
         *  Creates a new area with name
         */
        bool createArea(const std::string &name);

        /**
         *  Creates a room based on provided arguments
         */
        bool createRoom(const std::vector<std::string> &arguments);

        /**
         *  Creates an object in area containing roomId based on provided arguments
         */
        bool createObject(const model::ID &roomId, const std::vector<std::string> &arguments);

        /**
         *  Creates a npc in area containing roomId based on provided arguments
         */
        bool createNpc(const model::ID &roomId, const std::vector<std::string> &arguments);

        /**
         *  Creates an object reset based on roomId and provided arguments
         */
        bool createObjectReset(const model::ID &roomId, const std::vector<std::string> &arguments);

        /**
         *  Creates a npc reset based on roomId and provided arguments
         */
        bool createNpcReset(const model::ID &roomId, const std::vector<std::string> &arguments);

        /**
         *  Edits area containing roomId based on provided arguments
         */
        bool editArea(const model::ID &roomId, const std::vector<std::string> &arguments);

        /**
         *  Edits room with roomId based on provided arguments
         */
        bool editRoom(const model::ID &roomId, const std::vector<std::string> &arguments);

        /**
         *  Edits object in area containing roomId based on provided arguments
         */
        bool editObject(const model::ID &roomId, const std::vector<std::string> &arguments);

        /**
         *  Edits npc in area containing roomId based on provided arguments
         */
        bool editNpc(const model::ID &roomId, const std::vector<std::string> &arguments);

        /**
         *  Clears room with roomId of description, doors, objects, npcs, and extras,
         *  and removes all roomId resets in containing area
         */
        void clear(const model::ID &roomId);

        /**
         *  Resets area containing roomId
         */
        void resetArea(const model::ID &roomId);

        /**
         *  Resets world
         */
        void reset();

    private:
        World world;
        ResetHandler resetHandler;

        /**
         *  Checks if string is a number
         */
        bool isNum(const std::string &str) const;
    };
}

#endif //WORLDHANDLER_H
