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
                        std::cout << "NPC " << npc->getId() << " added to Room " << room->getId() << std::endl;
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
                        std::cout << "Object " << object->getId() << " added to Room " << room->getId() << std::endl;
                    }

                }
            }
        }


        void parseDataJson(const std::string& filePath) {

            std::ifstream ifs(filePath);
            json t = json::parse(ifs);


            Area area;
            std::vector<Room> rooms;
            std::vector<NPC> npcs;
            std::vector<Object> objects;
            std::vector<Reset> resets;


            if(t.find("AREA") != t.end()){
                area = t.at("AREA").get<Area>();
            }

            if(t.find("ROOMS") != t.end()){
                rooms = t.at("ROOMS").get<std::vector<Room>>();
            }

            if(t.find("RESETS") != t.end()){
                resets = t.at("RESETS").get<std::vector<Reset>>();
            }

            if(t.find("NPCS") != t.end()){
                npcs = t.at("NPCS").get<std::vector<NPC>>();
                addNPCsToRooms(rooms, npcs, resets);
            }

            if(t.find("OBJECTS") != t.end()){
                objects = t.at("OBJECTS").get<std::vector<Object>>();
                addObjectsToRooms(rooms, objects, resets);
            }

            if(t.find("HELPS") != t.end()){
                // HELPS field if empty in all json sample data
            }

            if(t.find("SHOPS") != t.end()){
                // SHOPS field if empty in all json sample data
            }

            area.setRooms(rooms);
            std::cout << area << std::endl;
        }

        void parseUsersJson(const std::string& filePath, World& world) {
            std::ifstream ifs(filePath);

            json usersJson = json::parse(ifs);
            json users = usersJson["USERS"];

            // TODO: Refactor this to be a ranged for loop
            for (json::iterator it = users.begin(); it != users.end(); ++it) {
                Player p (it.value().at("id"), it.value().at("username"), "");
                world.insertUser(p);
            }
        }

    } // anonymous namespace

    void ParseDataFile(const std::string& filePath){
        if(filesystem::extension(filePath) == JSON_EXTENSION){
            parseDataJson(filePath);
        }
    }

    void ParseUsersFile(const std::string& filePath, World& world){
        std::string extension = boost::filesystem::extension(filePath);

        if(extension == JSON_EXTENSION){
            parseUsersJson(filePath, world);
        }
    }
} // DataManager namespace

