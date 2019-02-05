//
// Created by jnhkm on 2019-02-04.
//

#ifndef WEBSOCKETNETWORKING_PLAYERACTION_H
#define WEBSOCKETNETWORKING_PLAYERACTION_H

#include "Player.h"

using model::Player;
using model::Object;
using model::Slot;

namespace action {
    class PlayerAction {
    private:
        PlayerAction();
    public:
        static void equipItem(Player &player, Object item);

        static void unequipSlot(Player &player, const Slot &slot);

        static void swapItems(Object item1, Object item2);

        static void addToInventoryItems(Player &player, Object item);

        static Object dropItemFromInventory(Player &player, Object item);

        static Object dropItemFromEquipped(Player &player, Slot slot);
    };
}

#endif //WEBSOCKETNETWORKING_PLAYERACTION_H
