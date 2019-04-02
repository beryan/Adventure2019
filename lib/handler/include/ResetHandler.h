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
constexpr int DEFAULT_RESET_INTERVAL = 30;

namespace handler {
    class ResetHandler {
    private:
        ResetLogic logic{};
        int resetInterval;
        int cyclesUntilReset;

    public:
        ResetHandler();

        void addNpcsToRooms(Area& area);

        void addObjectsToRooms(Area& area);

        void loadSavedResets(Area& area);

        void resetArea(Area& area);

        bool isTimeToReset();

        void decrementTimer();

        void resetTimer();
    };
}
#endif //RESETHANDLER_H
