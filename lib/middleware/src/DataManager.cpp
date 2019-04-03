//
// Created by arehal on 1/30/19.
//

#include "DataManager.h"
#include "World.h"
#include "Reset.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <boost/filesystem.hpp>

using Json = nlohmann::json;
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
constexpr auto USERS = "USERS";

typedef std::pair<model::ID, model::ID> IdPair;

namespace DataManager {

    namespace {

        Area parseAreaJson(Json inputJson) {

            bool checkFormat = (inputJson.find(AREA) != inputJson.end() && inputJson.find(ROOMS) != inputJson.end() && inputJson.find(RESETS) != inputJson.end() &&
                    inputJson.find(NPCS) != inputJson.end() && inputJson.find(OBJECTS) != inputJson.end());

            bool checkFields = (inputJson.at(AREA) != nullptr && inputJson.at(ROOMS) != nullptr && inputJson.at(RESETS) != nullptr &&
                    inputJson.at(NPCS) != nullptr && inputJson.at(OBJECTS) != nullptr);

            if( !checkFormat ){
                throw std::runtime_error("Incorrect format of load file.");

            }
            if( !checkFields ){
                throw std::runtime_error("JSON file contains null field(s)");
            }

            Area area = inputJson.at(AREA).get<Area>();
            area.setRooms(inputJson.at(ROOMS).get<std::vector<Room>>());

            auto resets = inputJson.at(RESETS).get<std::vector<Reset>>();
            auto objects = inputJson.at(OBJECTS).get<std::vector<Object>>();

            // Set correct slots for sample JSON equippable objects
            for (const auto &reset : resets) {
                if (reset.getAction() == "equip") {
                    auto objectId = reset.getId();

                    auto object_it = std::find_if(
                            objects.begin(),
                            objects.end(),
                            [&objectId](const auto &object) {
                                return object.getId() == objectId;
                            });

                    if (object_it != objects.end()) {
                        auto slot = model::getSlotFromJsonInt(reset.getSlot());
                        auto index = std::distance(objects.begin(), object_it);
                        objects.at(index).setSlot(slot);
                    }
                }
            }

            area.setResets(resets);
            area.setObjects(objects);
            area.setNpcs(inputJson.at(NPCS).get<std::vector<NPC>>());

            // save files contain a separate set of Resets
            if(inputJson.find(SAVERESETS) != inputJson.end()) {
                area.setSaveResets(inputJson.at(SAVERESETS).get<std::vector<Reset>>());
            }

            return area;
        }

        std::vector<Player> parseUsersJson(const std::string& filePath) {
            std::ifstream inFile(filePath);

            if (!inFile.is_open()) {
                throw std::runtime_error("Could not open file: " + filePath);
            }

            Json usersJson = Json::parse(inFile);

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

            Json inputJson = Json::parse(inFile);

            std::vector<Area> areas;

            for (auto it = inputJson.begin(); it != inputJson.end(); ++it) {
                areas.push_back(parseAreaJson(it.value()));
            }
            return areas;
        }

        std::vector<Reset> createSaveResets(Area area){
            std::vector<Reset> saveResets;
            std::multimap<IdPair, model::ID> npcMap;

            for(Room& room : area.getRooms()){
                for(auto& npc : room.getNpcs()){
                    npcMap.insert({{npc.getId(), room.getId()} , room.getId()});
                }

                model::ID roomId = room.getId();
                for(auto& object : room.getObjects()){
                    Reset room{"object", object.getId(), roomId};
                    saveResets.push_back(room);
                };
            }

            for(auto it = npcMap.cbegin(); it != npcMap.cend(); it = npcMap.upper_bound(it->first)) {
                int limit = npcMap.count(it->first);
                Reset room{"npc", it->first.first, limit, it->first.second};
                saveResets.push_back(room);
            };

            return saveResets;
        }

        void writeWorldToJson(World world){
            Json jsonAreas;

            for(auto& area : world.getAreas()) {
                Json jsonArea{{AREA, area}};

                jsonArea.push_back({ROOMS, area.getRooms()});
                jsonArea.push_back({NPCS, area.getNpcs()});
                jsonArea.push_back({OBJECTS, area.getObjects()});
                jsonArea.push_back({RESETS, area.getResets()});
                jsonArea.push_back({SAVERESETS, createSaveResets(area)});

                jsonAreas.push_back(jsonArea);
            }
            std::ofstream saveFile(SAVE_FILE_PATH);
            if (!saveFile.is_open()) {
                throw std::runtime_error("Could not open save file");
            }
            saveFile << std::setw(4) << jsonAreas << std::endl;
        }

        void saveUserToJson(Player& player){

            std::vector<Player> players;

            if(boost::filesystem::exists(REGISTERED_USERS_PATH)){
                std::ifstream inFile(REGISTERED_USERS_PATH);
                Json inputJson = Json::parse(inFile);
                players = inputJson.at(USERS).get<std::vector<Player>>();
            }

            auto it = std::find_if(players.begin(), players.end(),
                                   [&player](const Player &player2) {return player2.getId() == player.getId();});

            if(it->getId() == player.getId()) {
                players.erase(it);
            }

            players.push_back(player);

            Json users = Json{{USERS, players}};

            std::ofstream usersFile(REGISTERED_USERS_PATH);

            if(!usersFile.is_open()){
                throw std::runtime_error("Could not open users file");
            }
            usersFile << std::setw(4) << users << std::endl;
        }

        std::vector<Player> parseRegisteredUsers(Json json){
            return json.at(USERS).get<std::vector<Player>>();
        }

    } // anonymous namespace

    bool has_extension(const std::string &filePath, const std::string &extension) {
        if (filePath.length() >= extension.length()) {
            return filePath.compare (filePath.length() - extension.length(), extension.length(), extension) == 0;
        }

        return false;
    };

    Area ParseDataFile(const std::string& filePath){
        Area area;
        if(has_extension(filePath, JSON_EXTENSION)){
            std::ifstream inFile(filePath);

            if (!inFile.is_open()) {
                throw std::runtime_error("Could not open file: " + filePath);
            }

            Json inputJson = Json::parse(inFile);
            area = parseAreaJson(inputJson);
        }
        return area;
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

    void writeJson(Json j, std::string filePath) {
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

    void saveRegisteredUsers(const std::map<model::ID, Player>& players){
        for(auto player : players) {
            saveUserToJson(player.second);
        }
    }

    std::vector<Player> loadRegisteredPlayers(){
        std::vector<Player> players;
        if(boost::filesystem::exists(REGISTERED_USERS_PATH)) {
            std::ifstream inFile(REGISTERED_USERS_PATH);

            if (!inFile.is_open()) {
                throw std::runtime_error("Could not load registered users");
            }

            Json inputJson = Json::parse(inFile);

            players = parseRegisteredUsers(inputJson);
        }

        return players;
    }
} // DataManager namespace

