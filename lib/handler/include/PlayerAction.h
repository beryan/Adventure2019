//
// Created by jnhkm on 2019-02-04.
//

#ifndef PLAYERACTION_H
#define PLAYERACTION_H

#include "Player.h"
#include "PlayerLogic.h"

using model::Player;
using model::Object;
using model::Slot;
using logic::PlayerLogic;

namespace action {
    class PlayerAction {
    private:
        PlayerLogic logic{};
    public:
        PlayerAction();

        void equipItem(Player &player, Object item);

        void unequipSlot(Player &player, const Slot &slot);

        void pickupItem(Player &player, Object item);

        Object dropItemFromInventory(Player &player, Object item);

        Object dropItemFromEquipped(Player &player, Slot slot);
    };
}

#endif //PLAYERACTION_H
