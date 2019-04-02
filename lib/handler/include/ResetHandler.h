//
// Created by ARehal on 2019-03-19.
//

#ifndef RESETHANDLER_H
#define RESETHANDLER_H

#include "ResetLogic.h"
#include "Area.h"

using logic::ResetLogic;
using model::Area;

constexpr auto NPC_ACTION = "npc";
constexpr auto OBJECT_ACTION = "object";

namespace handler {
    class ResetHandler {
    private:
        ResetLogic logic{};
        int cyclesUntilReset = 30;

    public:
        ResetHandler();

        void addNpcsToRooms(Area& area);

        void addObjectsToRooms(Area& area);

        void loadSavedResets(Area& area);

        void resetArea(Area& area);

        void setResetInterval(int numberOfCycles);

        bool isTimeToReset();

        void decrementTimer();
    };
}
#endif //RESETHANDLER_H
