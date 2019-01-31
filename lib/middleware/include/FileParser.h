//
// Created by arehal on 1/29/19.
//

#ifndef WEBSOCKETNETWORKING_FILEPARSER_H
#define WEBSOCKETNETWORKING_FILEPARSER_H

#include <string>
#include <vector>

#include "Door.h"
#include "NPC.h"
#include "Object.h"
#include "Room.h"
#include "json.hpp"

using json = nlohmann::json;
using Door = model::Door;
using NPC = model::NPC;
using Object = model::Object;
using Room = model::Room;

const std::string JSON_EXTENSION = ".json";

namespace {
    std::vector<NPC> createNPCsFromJson(json npcsJson);
    std::vector<Object> createObjectsFromJson(json objectsJson);
    std::vector<Door> createDoorsFromJson(json doorsJson);
    std::vector<Room> createRoomsFromJson(json roomsJson);

    void parseJson(std::string filePath);
}

void ParseDataFile(std::string filePath);

#endif //WEBSOCKETNETWORKING_FILEPARSER_H
