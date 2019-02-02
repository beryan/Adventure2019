//
// Created by arehal on 1/30/19.
//

#include "DataManager.h"
#include "World.h"
#include "Reset.h"

#include <boost/filesystem.hpp>
#include <iostream>

using json = nlohmann::json;
using Door = model::Door;
using NPC = model::NPC;
using Object = model::Object;
using Player = model::Player;
using Reset = model::Reset;
using Room = model::Room;


namespace DataManager {

    namespace {

        std::vector<NPC> createNPCsFromJson(json npcsJson){
            std::vector<NPC> npcs;
            for(auto it : npcsJson.items()){
                std::vector<std::string> keywords = it.value()["keywords"];

                //TODO change to vector parse when NPC updated
                std::string desc = it.value().at("description").at(0);
                std::vector<std::string> longdesc = it.value()["longdesc"];

                NPC n (it.value().at("id"), keywords, desc, it.value().at("shortdesc"), longdesc);
                npcs.push_back(n);
            }
            return npcs;
        }

        std::vector<Object> createObjectsFromJson(json objectsJson) {
            std::vector<Object> objects;

            for (auto it : objectsJson.items()) {

                // objects dont have a name field, will use shortdesc as name for now
                std::string objName = it.value().at("shortdesc");
                std::vector<std::string> keywords = it.value()["keywords"];
                std::vector<std::string> longdesc = it.value()["longdesc"];

                Object o (it.value().at("id"), objName, it.value().at("shortdesc"), longdesc, keywords, model::Weapon);
                objects.push_back(o);

            }

            return objects;
        }

        std::vector<Door> createDoorsFromJson(json doorsJson) {
            std::vector<Door> doors;

            for(auto it : doorsJson.items()) {
                std::vector<std::string> desc = it.value()["desc"];
                std::vector<std::string> keywords = it.value()["keywords"];

                Door d {it.value().at("dir"), it.value().at("to"), desc, keywords};
                doors.push_back(d);
            }

            return doors;
        }

        std::vector<Room> createRoomsFromJson(json roomsJson) {
            std::vector<Room> rooms;
            
            for(auto it : roomsJson.items()) {
                std::vector<std::string> desc = it.value()["desc"];
                std::vector<Door> doors = createDoorsFromJson(it.value().at("doors"));

                Room r (it.value().at("id"), it.value().at("name"), desc);
                rooms.push_back(r);
            }

            return rooms;
        }

        std::vector<Reset> createResetsFromJson(json resetsJson) {
            std::vector<Reset> resets;

            for(auto it : resetsJson.items()) {
                Reset r;
                r.setAction(it.value().at("action"));
                r.setId(it.value().at("id"));

                if(it.value().find("limit") != it.value().end())
                {
                    r.setLimit(it.value().at("limit"));
                }

                if(it.value().find("room") != it.value().end())
                {
                    r.setLimit(it.value().at("room"));
                }

                if(it.value().find("slot") != it.value().end())
                {
                    r.setLimit(it.value().at("slot"));
                }

                resets.push_back(r);
            }

            return resets;
        }

        void parseDataJson(std::string filePath) {

            std::ifstream ifs(filePath);
            json t = json::parse(ifs);

            json areasJson = t["AREAS"];
            json roomsJson = t["ROOMS"];
            json npcsJson = t["NPCS"];
            json objectsJson = t["OBJECTS"];
//        json helpsJson = t["HELPS"];
            json resetsJson = t["RESETS"];
//        json shopsJson = t["SHOPS"];

            std::vector<NPC> npcs = createNPCsFromJson(npcsJson);
            std::cout << "Printing NPCs: " << std::endl;
            for(NPC n : npcs){
                std::cout << n;
            }
            std::cout << std::endl;

            std::vector<Object> objects = createObjectsFromJson(objectsJson);
            std::cout << "Printing Objects: " << std::endl;
            for(Object o : objects){
                std::cout << o;
            }
            std::cout << std::endl;

            std::vector<Room> rooms = createRoomsFromJson(roomsJson);
            std::cout << "Printing Rooms: " << std::endl;
            for(Room r : rooms) {
                std::cout << r;
            }
            std::cout << std::endl;

            std::vector<Reset> resets = createResetsFromJson(resetsJson);
            std::cout << "Printing Resets: " << std::endl;
            for(Reset r : resets) {
                std::cout << r;
            }


            std::cout << std::endl;

        }

        void parseUsersJson(std::string filePath, World& world) {
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

    void ParseDataFile(std::string filePath){
        std::string extension = boost::filesystem::extension(filePath);

        if(extension == JSON_EXTENSION){
            parseDataJson(filePath);
        }
    }

    void ParseUsersFile(std::string filePath, World& world){
        std::string extension = boost::filesystem::extension(filePath);

        if(extension == JSON_EXTENSION){
            parseUsersJson(filePath, world);
        }
    }
} // DataManager namespace

