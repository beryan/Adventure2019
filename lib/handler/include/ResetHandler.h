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
constexpr unsigned int MAX_ID_DIGITS = 4;
constexpr unsigned int MAX_NPC_OF_TYPE_COUNT = 20;
constexpr unsigned int DEFAULT_RESET_INTERVAL = 30;

namespace handler {
    class ResetHandler {
    private:
        ResetLogic logic{};
        unsigned int resetInterval;
        unsigned int cyclesUntilReset;

        int getAvailableNpcIndex(const model::ID &id, const Room &room);
        NPC createUniqueNpc(const NPC &npc, const Room &room);

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
