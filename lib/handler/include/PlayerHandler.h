//
// Created by jnhkm on 2019-02-04.
//

#ifndef PLAYERHANDLER_H
#define PLAYERHANDLER_H

#include "Player.h"
#include "PlayerLogic.h"

using model::Player;
using model::Object;
using model::Slot;
using logic::PlayerLogic;

namespace handler {
    class PlayerHandler {
    private:
        PlayerLogic logic{};
    public:
        PlayerHandler();

        /**
         * Equips an item directly from the inventory. It will do nothing if the item
         * does not exist in the inventory
         * @param player: Player that is equipping the item
         * @param item: Item that is being equipped
         * @return false if item cannot be equipped
         */
        bool equipItem(Player &player, const Object &item);

        /**
         * Unequipping the item will move the item from the Equipment to their Inventory. If the
         * item does not exist in Equipment, then the routine will do nothing
         * @param player: Player the item is being unequipped from
         * @param item: Item being unequipped
         */
        void unequipItem(Player &player, const Object &item);

        /**
         * Picks up and item from a room. Usually used in conjunction with Inventory's
         * removeItemFromInventory routine to get the item being picked up in the 2nd arg
         * @param player: Player picking up the item
         * @param item: Item being picked up
         */
        void pickupItem(Player &player, const Object &item);

        /**
         * Gives a specific item from giver to receiver if the item exists
         * @param giver: Player giving the item
         * @param receiver: Player receiving the item
         */
        void giveItem(Player &giver, Player &receiver, const Object &item);


        /**
         * Drops the item from either the player's Inventory or Equipment.
         * @param player: Player that is dropping the item
         * @param item: The item being dropped from the Player
         * @return If the item exists in either the Inventory or Equipment, it will return the
         * ownership of that item, else it will return ownership of an empty Object instance.
         * Usually used to drop and store item in a Room's inventory.
         */
        Object dropItem(Player &player, const Object &item);
    };
}

#endif //PLAYERHANDLER_H
