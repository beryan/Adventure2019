//
// Created by ARehal on 2019-03-19.
//

#include "ResetHandler.h"
#include <iostream>

using handler::ResetHandler;

namespace handler {
    ResetHandler::ResetHandler() {}

    void ResetHandler::addNpcsToRooms(Area& area){
        for(const Reset &r : area.getResets()) {
            if(r.getAction() == "npc") {
                auto room = area.findRoomById(r.getRoom());
                auto npc = area.findNpcById(r.getId());

                if(room != area.getRooms().end() && npc != area.getNpcs().end()) {
                    while(logic.canAddNpcToRoom(r, *room)){
                        room->addNPC(*npc);
                    }
                }

            }
        }
    }

    void ResetHandler::addObjectsToRooms(Area& area){
        for(Reset r : area.getResets()) {
            if(r.getAction() == "object") {
                auto room = area.findRoomById(r.getRoom());
                auto object = area.findObjectById(r.getId());

                if(room != area.getRooms().end() && object != area.getObjects().end()) {
                    if(logic.canAddObjectToRoom(r, *room)){
                        room->addObject(*object);
                    }
                }

            }
        }
    }


    void ResetHandler::resetArea(Area& area){
        addNpcsToRooms(area);
        addObjectsToRooms(area);
    }
}
