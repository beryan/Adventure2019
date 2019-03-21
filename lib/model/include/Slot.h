//
// Created by jnhkm on 2019-03-19.
//

#ifndef WEBSOCKETNETWORKING_SLOT_H
#define WEBSOCKETNETWORKING_SLOT_H

#include <unordered_map>
#include <string>
#include <algorithm>

namespace model {
    // Count returns the length of the Slots
    enum Slot{
        Head, Shoulders, Back, Chest, Hands, Legs, Feet, Weapon, Misc, Count
    };

    /**
     *
     * @param slot: String name of slot
     * @return Slot associated with the param if valid argument, else will return
     * Misc
     */
    Slot getSlotFromString(std::string slot);

    std::string getStringFromSlot(Slot slot);
}


#endif //WEBSOCKETNETWORKING_SLOT_H
