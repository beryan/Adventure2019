//
// Created by arehal on 1/30/19.
//

#include "FileParser.h"
#include "json.hpp"

#include <boost/filesystem.hpp>
#include <iostream>

using json = nlohmann::json;

namespace {

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
    }
} // namespace

void ParseDataFile(std::string filePath){

    std::string extension = boost::filesystem::extension(filePath);
    if(extension == JSON_EXTENSION){
        parseJson(filePath);
    }

}

