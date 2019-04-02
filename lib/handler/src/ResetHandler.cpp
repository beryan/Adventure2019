//
// Created by ARehal on 2019-03-19.
//

#include "ResetHandler.h"
#include <iostream>

using handler::ResetHandler;

namespace handler {
    ResetHandler::ResetHandler() {}

    void ResetHandler::addNpcsToRooms(Area& area){
        for(const Reset &reset : area.getResets()) {
            if(reset.getAction() == NPC_ACTION) {
                auto room = area.findRoomById(reset.getRoom());
                auto npc = area.findNpcById(reset.getId());

                if(room != area.getRooms().end() && npc != area.getNpcs().end()) {
                    while(logic.canAddNpcToRoom(reset, *room)){
                        room->addNPC(*npc);
                    }
                }

            }
        }
    }

    void ResetHandler::addObjectsToRooms(Area& area){
        for(Reset reset : area.getResets()) {
            if(reset.getAction() == OBJECT_ACTION) {
                auto room = area.findRoomById(reset.getRoom());
                auto object = area.findObjectById(reset.getId());

                bool roomAndObjectFound = (room != area.getRooms().end() && object != area.getObjects().end());

                if(roomAndObjectFound && logic.canAddObjectToRoom(reset, *room)){
                        room->addObject(*object);
                }
            }
        }
    }

    void ResetHandler::loadSavedResets(Area& area){
        for(const Reset &reset : area.getSaveResets()) {
            if(reset.getAction() == NPC_ACTION) {
                auto room = area.findRoomById(reset.getRoom());
                auto npc = area.findNpcById(reset.getId());

                if(room != area.getRooms().end() && npc != area.getNpcs().end()) {
                    while(logic.canAddNpcToRoom(reset, *room)){
                        room->addNPC(*npc);
                    }
                }
            }else if(reset.getAction() == OBJECT_ACTION) {
                auto room = area.findRoomById(reset.getRoom());
                auto object = area.findObjectById(reset.getId());

                bool roomAndObjectFound = (room != area.getRooms().end() && object != area.getObjects().end());

                if(roomAndObjectFound && logic.canAddObjectToRoom(reset, *room)){
                    room->addObject(*object);
                }
            }
        }
    }

    void ResetHandler::resetArea(Area& area){
        addNpcsToRooms(area);
        addObjectsToRooms(area);
    }

    void ResetHandler::setResetInterval(int numberOfCycles) {
        this->cyclesUntilReset = numberOfCycles;
    }

    bool ResetHandler::isTimeToReset() {
        return this->cyclesUntilReset <= 0;
    }

    void ResetHandler::decrementTimer() {
        --this->cyclesUntilReset;
    }

}
