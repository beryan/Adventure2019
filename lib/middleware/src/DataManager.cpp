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

                std::cout << "Printing Rooms: " << std::endl;
                for(Room r : rooms) {
                    std::cout << r;
                }
                std::cout << std::endl;
            }

            if(t.find("RESETS") != t.end()){
                resets = t.at("RESETS").get<std::vector<Reset>>();

                std::cout << "Printing Resets: " << std::endl;
                for(Reset r : resets) {
                    std::cout << r;
                }
                std::cout << std::endl;
            }

            if(t.find("NPCS") != t.end()){
                npcs = t.at("NPCS").get<std::vector<NPC>>();

                std::cout << "Printing NPCs: " << std::endl;
                for(NPC n : npcs){
                    std::cout << n;
                }
                std::cout << std::endl;
            }

            if(t.find("OBJECTS") != t.end()){
                objects = t.at("OBJECTS").get<std::vector<Object>>();

                std::cout << "Printing Objects: " << std::endl;
                for(Object o : objects) {
                    std::cout << o;
                }
                std::cout << std::endl;
            }

            if(t.find("HELPS") != t.end()){
                // HELPS field if empty in all json sample data
            }

            if(t.find("SHOPS") != t.end()){
                // SHOPS field if empty in all json sample data
            }

            area.setRooms(rooms);
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

