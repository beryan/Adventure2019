//
// Created by ARehal on 2019-03-19.
//

#include "ResetHandler.h"
#include "PropertiesManager.h"
#include <iostream>
#include <sstream>

using handler::ResetHandler;
using handler::PropertiesManager;

namespace handler {
    ResetHandler::ResetHandler() {
        int resetIntervalProperty = DEFAULT_RESET_INTERVAL;
        PropertiesManager::getProperty(PropertiesManager::RESET_INTERVAL_PROPERTY_NAME, resetIntervalProperty);
        resetInterval = static_cast<unsigned int>(resetIntervalProperty);
        cyclesUntilReset = resetInterval;
    }

    void ResetHandler::addNpcsToRooms(Area& area){
        for (const auto &reset : area.getResets()) {
            if (reset.getAction() == NPC_ACTION) {
                auto room = area.findRoomById(reset.getRoom());
                auto npc = area.findNpcById(reset.getId());

                if (room != area.getRooms().end() && npc != area.getNpcs().end()) {
                    if (logic.canAddNpcToRoom(reset, *room)) {
                        NPC newNpc{
                            npc->getId(),
                            npc->getKeywords(),
                            npc->getDescription(),
                            npc->getShortDescription(),
                            npc->getLongDescription()
                        };

                        auto count = room->countNpcById(reset.getId()) + 1;
                        std::ostringstream uniqueIdString;
                        uniqueIdString << std::to_string(count)
                                       << std::to_string(npc->getId())
                                       << std::to_string(room->getId());
                        newNpc.setUniqueId(std::stoi(uniqueIdString.str()));
                        room->addNPC(newNpc);
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
                        NPC newNpc{
                            npc->getId(),
                            npc->getKeywords(),
                            npc->getDescription(),
                            npc->getShortDescription(),
                            npc->getLongDescription()
                        };

                        auto count = room->countNpcById(reset.getId()) + 1;
                        std::ostringstream uniqueIdString;
                        uniqueIdString << std::to_string(count)
                                       << std::to_string(npc->getId())
                                       << std::to_string(room->getId());
                        newNpc.setUniqueId(std::stoi(uniqueIdString.str()));
                        room->addNPC(newNpc);
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
