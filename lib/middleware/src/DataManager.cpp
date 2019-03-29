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

        void saveUserToJson(Player& p){

            std::vector<Player> players;

            if(boost::filesystem::exists(REGISTERED_USERS_PATH)){
                std::ifstream inFile(REGISTERED_USERS_PATH);
                json t = json::parse(inFile);
                players = t.at(USERS).get<std::vector<Player>>();
            }

            players.push_back(p);

            json users = json{{USERS, players}};

            std::ofstream usersFile(REGISTERED_USERS_PATH);

            if(!usersFile.is_open()){
                throw std::runtime_error("Could not open users file");
            }
            usersFile << std::setw(4) << users << std::endl;
        }


        std::vector<Player> parseRegisteredUsers(json j){
            return j.at(USERS).get<std::vector<Player>>();
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

    void saveRegisteredUser(Player p){
        saveUserToJson(p);
    }

    std::vector<Player> loadRegisteredPlayers(){
        std::vector<Player> players;
        if(boost::filesystem::exists(REGISTERED_USERS_PATH)) {
            std::ifstream inFile(REGISTERED_USERS_PATH);

            if (!inFile.is_open()) {
                throw std::runtime_error("Could not load registered users");
            }

            json j = json::parse(inFile);

            players = parseRegisteredUsers(j);
        }

        return players;
    }

    void moveUsersFileToSrcDir(){
        //copy users.json to adventure/
        if(boost::filesystem::exists(REGISTERED_USERS_PATH)) {
            boost::filesystem::path adventureDir{"../adventure2019/lib/data/users.json"};
            boost::filesystem::path buildDir{REGISTERED_USERS_PATH};
            boost::filesystem::copy_file(buildDir, adventureDir, boost::filesystem::copy_option::overwrite_if_exists);
        }
    }

} // DataManager namespace

