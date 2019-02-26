//
// Created by arehal on 1/30/19.
//

#include "DataManager.h"
#include "World.h"
#include "Reset.h"

#include <boost/filesystem.hpp>
#include <iostream>

namespace filesystem = boost::filesystem;

using json = nlohmann::json;
using Door = model::Door;
using NPC = model::NPC;
using Object = model::Object;
using Player = model::Player;
using Reset = model::Reset;
using Room = model::Room;

const std::string AREA = "AREA";
const std::string ROOMS = "ROOMS";
const std::string RESETS = "RESETS";
const std::string NPCS = "NPCS";
const std::string OBJECTS = "OBJECTS";
const std::string HELPS = "HELPS";
const std::string SHOPS = "SHOPS";
const std::string USERS = "USERS";

namespace DataManager {

    namespace {

        void addNPCsToRooms(std::vector<Room>& rooms, std::vector<NPC>& npcs, const std::vector<Reset>& resets) {
            for(Reset r : resets) {
                if(r.getAction() == "npc") {
                    auto roomID = r.getRoom();
                    auto room = std::find_if(rooms.begin(), rooms.end(),
                                             [roomID](const Room & room) -> bool { return room.getId() == roomID ; });

                    auto npcID = r.getId();
                    auto npc = std::find_if(npcs.begin(), npcs.end(),
                                            [npcID](const NPC & n) -> bool { return n.getId() == npcID ; });

                    if(room != rooms.end() && npc != npcs.end()) {
                        room->addNPC(*npc);
                    }

                }
            }
        }

        void addObjectsToRooms(std::vector<Room>& rooms, std::vector<Object>& objects, const std::vector<Reset>& resets) {
            for(Reset r : resets) {
                if(r.getAction() == "object") {
                    auto roomID = r.getRoom();
                    auto room = std::find_if(rooms.begin(), rooms.end(),
                                             [roomID](const Room & room) -> bool { return room.getId() == roomID ; });

                    auto objectID = r.getId();
                    auto object = std::find_if(objects.begin(), objects.end(),
                                               [objectID](const Object & o) -> bool { return o.getId() == objectID ; });

                    if(room != rooms.end() && object != objects.end()) {
                        room->addObject(*object);
                    }

                }
            }
        }


        Area parseDataJson(const std::string& filePath) {

            std::ifstream ifs(filePath);
            json t = json::parse(ifs);

            Area area;
            std::vector<Room> rooms;
            std::vector<NPC> npcs;
            std::vector<Object> objects;
            std::vector<Reset> resets;


            if(t.find(AREA) != t.end()){
                area = t.at(AREA).get<Area>();
            }

            if(t.find(ROOMS) != t.end()){
                rooms = t.at(ROOMS).get<std::vector<Room>>();
            }

            if(t.find(RESETS) != t.end()){
                resets = t.at(RESETS).get<std::vector<Reset>>();
            }

            if(t.find(NPCS) != t.end()){
                npcs = t.at(NPCS).get<std::vector<NPC>>();
                addNPCsToRooms(rooms, npcs, resets);
            }

            if(t.find(OBJECTS) != t.end()){
                objects = t.at(OBJECTS).get<std::vector<Object>>();
                addObjectsToRooms(rooms, objects, resets);
            }

            if(t.find(HELPS) != t.end()){
                // HELPS field if empty in all json sample data
            }

            if(t.find(SHOPS) != t.end()){
                // SHOPS field if empty in all json sample data
            }

            area.setRooms(rooms);
            return area;
        }

        std::vector<Player> parseUsersJson(const std::string& filePath) {
            std::ifstream ifs(filePath);
            json usersJson = json::parse(ifs);

            std::vector<Player> players;

            if(usersJson.find(USERS) != usersJson.end()){
                players = usersJson.at(USERS).get<std::vector<Player>>();
            }
            return players;
        }

    } // anonymous namespace

    Area ParseDataFile(const std::string& filePath){
        Area a;
        if(filesystem::extension(filePath) == JSON_EXTENSION){
            a = parseDataJson(filePath);
        }
        return a;
    }

    std::vector<Player> ParseUsersFile(const std::string& filePath){
        std::vector<Player> players;
        if(filesystem::extension(filePath) == JSON_EXTENSION){
            players = parseUsersJson(filePath);
        }
        return players;
    }
} // DataManager namespace

