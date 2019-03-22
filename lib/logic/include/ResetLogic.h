//
// Created by ARehal on 2019-03-19.
//

#ifndef RESETLOGIC_H
#define RESETLOGIC_H

#include "Reset.h"
#include "Room.h"

using model::Reset;
using model::Room;

namespace logic {
    class ResetLogic {
    public:
        ResetLogic();

        bool canAddNpcToRoom(const Reset& reset, const Room& room) const;
        bool canAddObjectToRoom(const Reset& reset, const Room& room) const;
    };

}

#endif //RESETLOGIC_H
