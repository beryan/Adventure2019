//
// Created by arehal on 1/29/19.
//

#ifndef WEBSOCKETNETWORKING_FILEPARSER_H
#define WEBSOCKETNETWORKING_FILEPARSER_H

#include <string>
#include <vector>

#include "NPC.h"
#include "json.hpp"

using json = nlohmann::json;

const std::string JSON_EXTENSION = ".json";

namespace {
    std::vector<model::NPC> createNPCsFromJson(json npcsJson);
    void parseJson(std::string filePath);
}

void ParseDataFile(std::string filePath);

#endif //WEBSOCKETNETWORKING_FILEPARSER_H
