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
//        json helpsJson = t["HELPS"];
            json resetsJson = t["RESETS"];
//        json shopsJson = t["SHOPS"];

            std::vector<NPC> npcs = t.at("NPCS").get<std::vector<NPC>>();
            std::cout << "Printing NPCs: " << std::endl;
            for(NPC n : npcs){
                std::cout << n;
            }
            std::cout << std::endl;

            std::vector<Object> objects = t.at("OBJECTS").get<std::vector<Object>>();
            std::cout << "Printing Objects: " << std::endl;
            for(Object o : objects) {
                std::cout << o;
            }
            std::cout << std::endl;

            std::vector<Room> rooms = t.at("ROOMS").get<std::vector<Room>>();
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

