//
// Created by ARehal on 2019-03-19.
//

#include "ResetLogic.h"

using logic::ResetLogic;

namespace logic {
    ResetLogic::ResetLogic() {}

    bool ResetLogic::canAddNpcToRoom(Reset reset, Room room) const {
        return reset.getLimit() >= room.countNpcById(reset.getId());
    }

    bool ResetLogic::canAddObjectToRoom(Reset reset, Room room) const {
        return room.countObjectById(reset.getId()) == 0;
    }
}
