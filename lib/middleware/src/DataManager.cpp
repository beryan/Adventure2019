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

            Area area;
            std::vector<Room> rooms;
            std::vector<NPC> npcs;
            std::vector<Object> objects;
            std::vector<Reset> resets;


            if(t.find(AREA) != t.end()){
                area = t.at(AREA).get<Area>();
            }

            if(t.find(ROOMS) != t.end()){
                area.setRooms(t.at(ROOMS).get<std::vector<Room>>());
            }

            if(t.find(RESETS) != t.end()){
                area.setResets(t.at(RESETS).get<std::vector<Reset>>());
            }

            if(t.find(NPCS) != t.end()){
                area.setNpcs(t.at(NPCS).get<std::vector<NPC>>());
            }

            if(t.find(OBJECTS) != t.end()){
                area.setObjects(t.at(OBJECTS).get<std::vector<Object>>());
            }

            if(t.find(HELPS) != t.end()){
                // HELPS field if empty in all json sample data
            }

            if(t.find(SHOPS) != t.end()){
                // SHOPS field if empty in all json sample data
            }

            area.addNPCsToRooms();
            area.addObjectsToRooms();

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
} // DataManager namespace

