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

        /**
         * Equips an item directly from the inventory. It will do nothing if the item
         * does not exist in the inventory
         * @param player: Player that is equipping the item
         * @param item: Item that is being equipped
         */
        void equipItem(Player &player, Object item);

        /**
         * Unequipping the item
         * @param player
         * @param item: Item being unequipped (passed by reference and not value because
         * the item is unequipped by referring to its slots)
         */
        void unequipItem(Player &player, Object item);

        void pickupItem(Player &player, Object item);

        Object dropItem(Player &player, Object item);
    };
}

#endif //PLAYERACTION_H
