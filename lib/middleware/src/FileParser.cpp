//
// Created by arehal on 1/30/19.
//

#include "FileParser.h"
#include "json.hpp"
#include "NPC.h"
#include "Object.h"

#include <boost/filesystem.hpp>
#include <iostream>

using json = nlohmann::json;
using NPC = model::NPC;
using Object = model::Object;

namespace {

    std::vector<NPC> createNPCsFromJson(json npcsJson){
        std::vector<NPC> npcs;
        for (json::iterator it = npcsJson.begin(); it != npcsJson.end(); ++it) {

            std::vector<std::string> keywords;
            for(std::string keyword : it.value().at("keywords")){
                keywords.push_back(keyword);
            }

// use this when NPC.description type is changed to vector
//            std::vector<std::string> desc;
//            for(json j : it.value().at("description")){
//                desc.push_back(j);
//            }

            std::string desc = it.value().at("description").at(0);

            std::vector<std::string> longdesc;
            for(json j : it.value().at("longdesc")){
                longdesc.push_back(j);
            }

            NPC n (it.value().at("id"), keywords, desc, it.value().at("shortdesc"), longdesc);
            npcs.push_back(n);
        }
        return npcs;
    }

    std::vector<Object> createObjectsFromJson(json objectsJson) {
        std::vector<Object> objects;

        for (json::iterator it = objectsJson.begin(); it != objectsJson.end(); ++it) {

            std::vector<std::string> keywords;
            for (std::string keyword : it.value().at("keywords")) {
                keywords.push_back(keyword);
            }

            std::vector<std::string> longdesc;
            for (json j : it.value().at("longdesc")) {
                longdesc.push_back(j);
            }

//            std::vector<std::string> extras;
//            for (json j : it.value().at("extra")) {
//                extras.push_back(j);
//            }

            // Object is missing some fields
            Object o (it.value().at("id"), it.value().at("shortdesc"), it.value().at("longdesc").at(0), model::Weapon);
            objects.push_back(o);

        }

        return objects;
    }

    std::vector<Door> createDoorsFromJson(json doorsJson) {
        std::vector<Door> doors;

        for(json::iterator it = doorsJson.begin(); it != doorsJson.end(); ++it) {

            std::vector<std::string> desc;
            for (json j : it.value().at("desc")) {
                desc.push_back(j);
            }

            std::vector<std::string> keywords;
            for (std::string keyword : it.value().at("keywords")) {
                keywords.push_back(keyword);
            }

            Door d (it.value().at("dir"), desc, keywords, it.value().at("to"));
            doors.push_back(d);
        }

        return doors;
    }
    void parseJson(std::string filePath) {

        std::ifstream ifs(filePath);

        json t = json::parse(ifs);

        json areasJson;
        json roomJson;
        json npcsJson;
        json objectsJson;
//        json helpsJson;
        json resetsJson;
//        json shopsJson;


        for (json::iterator it = t.begin(); it != t.end(); ++it) {
            if (it.key() == "AREAS") {
                areasJson = it.value();
            } else if (it.key() == "ROOM") {
                roomJson = it.value();
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
        for(NPC n : npcs){
            std::cout << n;
        }

        std::vector<Object> objects = createObjectsFromJson(objectsJson);
        for(Object o : objects){
            std::cout << o;
        }

    }
} // namespace

void ParseDataFile(std::string filePath){

    std::string extension = boost::filesystem::extension(filePath);
    if(extension == JSON_EXTENSION){
        parseJson(filePath);
    }

}

