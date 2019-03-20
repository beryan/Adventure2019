//
// Created by ARehal on 2019-03-19.
//

#ifndef RESETHANDLER_H
#define RESETHANDLER_H

#include "ResetLogic.h"
#include "Area.h"

using logic::ResetLogic;
using model::Area;

namespace handler {
    class ResetHandler {
    private:
        ResetLogic logic{};
    public:
        ResetHandler();

        void addNpcsToRooms(Area& area);

        void addObjectsToRooms(Area& area);

        void resetArea(Area& area);
    };
}
#endif //RESETHANDLER_H
