//
// Created by arehal on 1/30/19.
//

#include "DataManager.h"
#include "World.h"
#include "Reset.h"

#include <iostream>
#include <fstream>
#include <iomanip>

using json = nlohmann::json;
using Door = model::Door;
using NPC = model::NPC;
using Object = model::Object;
using Player = model::Player;
using Reset = model::Reset;
using Room = model::Room;

constexpr auto AREA = "AREA";
constexpr auto ROOMS = "ROOMS";
constexpr auto RESETS = "RESETS";
constexpr auto SAVERESETS = "SAVERESETS";
constexpr auto NPCS = "NPCS";
constexpr auto OBJECTS = "OBJECTS";
constexpr auto HELPS = "HELPS";
constexpr auto SHOPS = "SHOPS";
constexpr auto USERS = "USERS";

typedef std::pair<model::ID, model::ID> IdPair;

namespace DataManager {

    namespace {

        Area parseAreaJson(json t) {

            bool checkFormat = (t.find(AREA) != t.end() && t.find(ROOMS) != t.end() && t.find(RESETS) != t.end() &&
                                t.find(NPCS) != t.end() && t.find(OBJECTS) != t.end());

            bool checkFields = (t.at(AREA) != nullptr && t.at(ROOMS) != nullptr && t.at(RESETS) != nullptr &&
                                t.at(NPCS) != nullptr && t.at(OBJECTS) != nullptr);

            if( !checkFormat ){
                throw std::runtime_error("Incorrect format of load file.");

            }
            if( !checkFields ){
                throw std::runtime_error("JSON file contains null field(s)");
            }

            Area area = t.at(AREA).get<Area>();
            area.setRooms(t.at(ROOMS).get<std::vector<Room>>());
            area.setResets(t.at(RESETS).get<std::vector<Reset>>());
            area.setNpcs(t.at(NPCS).get<std::vector<NPC>>());
            area.setObjects(t.at(OBJECTS).get<std::vector<Object>>());

            // save files contain a separate set of Resets
            if(t.find(SAVERESETS) != t.end()) {
                area.setSaveResets(t.at(SAVERESETS).get<std::vector<Reset>>());
            }

            return area;
        }

        std::vector<Player> parseUsersJson(const std::string& filePath) {
            std::ifstream inFile(filePath);

            if (!inFile.is_open()) {
                throw std::runtime_error("Could not open file: " + filePath);
            }

            json usersJson = json::parse(inFile);

            std::vector<Player> players;

            if(usersJson.find(USERS) != usersJson.end()){
                players = usersJson.at(USERS).get<std::vector<Player>>();
            }
            return players;
        }

        std::vector<Area> parseWorldJson(const std::string& filePath){
            // read a JSON file
            std::ifstream inFile(filePath);

            if (!inFile.is_open()) {
                throw std::runtime_error("Could not open file: " + filePath);
            }

            json j = json::parse(inFile);

            std::vector<Area> areas;

            for (auto it = j.begin(); it != j.end(); ++it) {
                areas.push_back(parseAreaJson(it.value()));
            }
            return areas;
        }


        std::vector<Reset> createSaveResets(Area a){
            std::vector<Reset> saveResets;
            std::multimap<IdPair, model::ID> npcMap;

            for(Room& r : a.getRooms()){
                for(auto& n : r.getNpcs()){
                    npcMap.insert({{n.getId(), r.getId()} , r.getId()});
                }

                model::ID roomId = r.getId();
                for(auto& o : r.getObjects()){
                    Reset r{"object", o.getId(), roomId};
                    saveResets.push_back(r);
                };
            }

            for(auto it = npcMap.cbegin(); it != npcMap.cend(); it = npcMap.upper_bound(it->first)) {
                int limit = npcMap.count(it->first);
                Reset r{"npc", it->first.first, limit, it->first.second};
                saveResets.push_back(r);
            };

            return saveResets;
        }

        void writeWorldToJson(World w){
            json areas;

            for(auto& a : w.getAreas()) {
                json area = json{{AREA, a}};

                area.push_back({ROOMS, a.getRooms()});
                area.push_back({NPCS, a.getNpcs()});
                area.push_back({OBJECTS, a.getObjects()});
                area.push_back({RESETS, a.getResets()});
                area.push_back({SAVERESETS, createSaveResets(a)});

                areas.push_back(area);
            }
            std::ofstream saveFile(SAVE_FILE_PATH);
            if (!saveFile.is_open()) {
                throw std::runtime_error("Could not open save file");
            }
            saveFile << std::setw(4) << areas << std::endl;
        }

    } // anonymous namespace

    bool has_extension(const std::string &filePath, const std::string &extension) {
        if (filePath.length() >= extension.length()) {
            return filePath.compare (filePath.length() - extension.length(), extension.length(), extension) == 0;
        }

        return false;
    };

    Area ParseDataFile(const std::string& filePath){
        Area a;
        if(has_extension(filePath, JSON_EXTENSION)){
            std::ifstream inFile(filePath);

            if (!inFile.is_open()) {
                throw std::runtime_error("Could not open file: " + filePath);
            }

            json j = json::parse(inFile);
            a = parseAreaJson(j);
        }
        return a;
    }

    std::vector<Player> ParseUsersFile(const std::string& filePath){
        std::vector<Player> players;
        if(has_extension(filePath, JSON_EXTENSION)){
            players = DataManager::parseUsersJson(filePath);
        }
        return players;
    }

    std::vector<Area> ParseWorldFile(const std::string& filePath) {
        std::vector<Area> areas;
        if(has_extension(filePath, JSON_EXTENSION)) {
            areas = parseWorldJson(filePath);
        }
        return areas;
    }

    void writeJson(json j, std::string filePath) {
        std::ofstream outFile(filePath);
        if (!outFile.is_open()) {
            throw std::runtime_error("Could not open file: " + filePath);
        }

        outFile << std::setw(2) << j;
    }

    void writeWorldToFile(World& world, FileType type) {
        if(type == DataManager::JSON) {
            writeWorldToJson(world);
        }
    }

} // DataManager namespace

