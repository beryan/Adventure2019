//
// Created by arehal on 1/30/19.
//

#include "FileParser.h"

#include "Door.h"
#include "NPC.h"
#include "Object.h"
#include "Room.h"

#include <boost/filesystem.hpp>
#include <iostream>

using json = nlohmann::json;
using Door = model::Door;
using NPC = model::NPC;
using Object = model::Object;
using Room = model::Room;

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

            std::vector<std::string> keywords = it.value()["keywords"];

            std::vector<std::string> longdesc = it.value()["longdesc"];

            //TODO parse missing fields

            Object o (it.value().at("id"), it.value().at("shortdesc"), it.value().at("longdesc").at(0), model::Weapon);
            objects.push_back(o);

        }

        return objects;
    }

    std::vector<Door> createDoorsFromJson(json doorsJson) {
        std::vector<Door> doors;

        for(auto it : doorsJson.items()) {

            std::vector<std::string> desc = it.value()["desc"];

            std::vector<std::string> keywords = it.value()["keywords"];

            Door d (it.value().at("dir"), desc, keywords, it.value().at("to"));
            doors.push_back(d);
        }

        return doors;
    }

    std::vector<Room> createRoomsFromJson(json roomsJson) {
        std::vector<Room> rooms;
        for(auto it : roomsJson.items()) {
            std::vector<std::string> desc = it.value()["desc"];

            std::vector<Door> doors = createDoorsFromJson(it.value().at("doors"));

            Room r (it.value().at("id"), it.value().at("name"), desc, doors);
            rooms.push_back(r);
        }

        return rooms;
    }


    void parseJson(std::string filePath) {

        std::ifstream ifs(filePath);

        json t = json::parse(ifs);

        json areasJson;
        json roomsJson;
        json npcsJson;
        json objectsJson;
//        json helpsJson;
        json resetsJson;
//        json shopsJson;


        for (json::iterator it = t.begin(); it != t.end(); ++it) {
            if (it.key() == "AREAS") {
                areasJson = it.value();
            } else if (it.key() == "ROOMS") {
                roomsJson = it.value();
            } else if (it.key() == "NPCS") {
                npcsJson = it.value();
            } else if (it.key() == "OBJECTS") {
                objectsJson = it.value();
            } else if (it.key() == "HELPS") {
//                helpsJson = it.value();
            } else if (it.key() == "RESETS") {
                resetsJson = it.value();
            } else if (it.key() == "SHOPS") {
//                shopsJson = it.value();
            }
        }
        
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
        std::cout << "size of rooms: " << rooms.size() << std::endl;
        for(Room r : rooms) {
            std::cout << r;
        }
        std::cout << std::endl;

    }
} // namespace

void ParseDataFile(std::string filePath){

    std::string extension = boost::filesystem::extension(filePath);
    if(extension == JSON_EXTENSION){
        parseJson(filePath);
    }

}

