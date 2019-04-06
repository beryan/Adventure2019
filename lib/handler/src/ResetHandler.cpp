//
// Created by ARehal on 2019-03-19.
//

#include "ResetHandler.h"
#include "PropertiesManager.h"
#include <sstream>
#include <unordered_set>

using handler::ResetHandler;
using handler::PropertiesManager;

namespace handler {
    ResetHandler::ResetHandler() {
        int resetIntervalProperty = DEFAULT_RESET_INTERVAL;
        PropertiesManager::getProperty(PropertiesManager::RESET_INTERVAL_PROPERTY_NAME, resetIntervalProperty);
        resetInterval = static_cast<unsigned int>(resetIntervalProperty);
        cyclesUntilReset = resetInterval;
    }

    int ResetHandler::getAvailableNpcIndex(const model::ID &id, const Room &room) {
        auto npcs = room.getImmutableNpcs();
        auto baseUniqueSize = (std::to_string(id) + std::to_string(room.getId())).size();
        std::unordered_set<int> currentIndices;
        for (const auto &npc : npcs) {
            if (npc.getId() == id) {
                auto uniqueIdString = std::to_string(npc.getUniqueId());
                auto baseStartIndex = uniqueIdString.size() - baseUniqueSize;
                auto index = std::stoi(uniqueIdString.substr(0, baseStartIndex));
                currentIndices.emplace(index);
            }
        }

        for (unsigned int index = 0; index <= npcs.size(); ++index) {
            if (!currentIndices.count(index + 1)) {
                return index + 1;
            }
        }

        return 1;
    }

    NPC ResetHandler::createUniqueNpc(const NPC &npc, const Room &room) {
        int index = this->getAvailableNpcIndex(npc.getId(), room);

        std::ostringstream uniqueIdString;
        uniqueIdString << std::to_string(index)
                       << std::to_string(npc.getId())
                       << std::to_string(room.getId());

        std::vector<std::string> keywords;
        for (auto &keyword : npc.getKeywords()) {
            keywords.push_back(keyword);
            keywords.push_back(keyword + std::to_string(index));
        }

        std::ostringstream shortDescription;
        shortDescription << npc.getShortDescription() << " (" << std::to_string(index) << ")";

        return NPC{
            npc.getId(),
            std::stoi(uniqueIdString.str()),
            keywords,
            npc.getDescription(),
            shortDescription.str(),
            npc.getLongDescription()
        };
    }

    void ResetHandler::addNpcsToRooms(Area& area){
        for (const auto &reset : area.getResets()) {
            if (reset.getAction() == NPC_ACTION) {
                auto room = area.findRoomById(reset.getRoom());
                auto npc = area.findNpcById(reset.getId());

                if (room != area.getRooms().end() && npc != area.getNpcs().end()) {
                    if (logic.canAddNpcToRoom(reset, *room)) {
                        room->addNPC(this->createUniqueNpc(*npc, *room));
                    }
                }

            }
        }
    }

    void ResetHandler::addObjectsToRooms(Area& area){
        for (const auto &reset : area.getResets()) {
            if (reset.getAction() == OBJECT_ACTION) {
                auto room = area.findRoomById(reset.getRoom());
                auto object = area.findObjectById(reset.getId());

                bool roomAndObjectFound = (room != area.getRooms().end() && object != area.getObjects().end());

                if (roomAndObjectFound && logic.canAddObjectToRoom(reset, *room)) {
                        room->addObject(*object);
                }
            }
        }
    }

    void ResetHandler::loadSavedResets(Area& area){
        for (const auto &reset : area.getSaveResets()) {
            if (reset.getAction() == NPC_ACTION) {
                auto room = area.findRoomById(reset.getRoom());
                auto npc = area.findNpcById(reset.getId());

                if (room != area.getRooms().end() && npc != area.getNpcs().end()) {
                    while (logic.canAddNpcToRoom(reset, *room)) {
                        room->addNPC(this->createUniqueNpc(*npc, *room));
                    }
                }
            } else if (reset.getAction() == OBJECT_ACTION) {
                auto room = area.findRoomById(reset.getRoom());
                auto object = area.findObjectById(reset.getId());

                bool roomAndObjectFound = (room != area.getRooms().end() && object != area.getObjects().end());

                if (roomAndObjectFound && logic.canAddObjectToRoom(reset, *room)) {
                    room->addObject(*object);
                }
            }
        }
    }

    void ResetHandler::resetArea(Area& area){
        addNpcsToRooms(area);
        addObjectsToRooms(area);
    }

    bool ResetHandler::isTimeToReset() {
        return this->cyclesUntilReset == 0;
    }

    void ResetHandler::decrementTimer() {
        if (this->cyclesUntilReset > 0) {
            --this->cyclesUntilReset;
        }
    }

    void ResetHandler::resetTimer() {
        this->cyclesUntilReset = this->resetInterval;
    }

}
