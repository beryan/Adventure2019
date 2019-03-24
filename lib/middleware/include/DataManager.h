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

constexpr auto JSON_EXTENSION = ".json";
constexpr auto REGISTERED_USERS_PATH = "../adventure2019/lib/data/users.json";

namespace DataManager {
    Area ParseDataFile(const std::string& filePath);
    std::vector<Player> ParseUsersFile(const std::string& filePath);
    std::vector<Area> ParseWorldFile(const std::string& filePath);
    void writeJson(json j, std::string filePath);
    void saveRegisteredUser(Player p);
}
#endif //WEBSOCKETNETWORKING_FILEPARSER_H
