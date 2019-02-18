//
// Created by arehal on 1/29/19.
//

#ifndef WEBSOCKETNETWORKING_FILEPARSER_H
#define WEBSOCKETNETWORKING_FILEPARSER_H

#include <string>

#include "json.hpp"
#include "World.h"

using model::World;
using model::Player;

const std::string JSON_EXTENSION = ".json";

namespace DataManager {
    Area ParseDataFile(const std::string& filePath);
    std::vector<Player> ParseUsersFile(const std::string& filePath);
}
#endif //WEBSOCKETNETWORKING_FILEPARSER_H
