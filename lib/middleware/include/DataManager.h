//
// Created by arehal on 1/29/19.
//

#ifndef WEBSOCKETNETWORKING_FILEPARSER_H
#define WEBSOCKETNETWORKING_FILEPARSER_H

#include <string>

#include "json.hpp"
#include "World.h"

using model::World;

const std::string JSON_EXTENSION = ".json";

namespace DataManager {
    void ParseDataFile(std::string filePath);
    void ParseUsersFile(std::string filePath, World& world);
}
#endif //WEBSOCKETNETWORKING_FILEPARSER_H
