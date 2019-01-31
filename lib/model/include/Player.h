//
// Created by jnhkm on 2019-01-18.
//

#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <array>
#include <map>
#include "Character.h"

namespace model {
    /**
    *  @class Player
    *
    *  @brief A struct describing a player.
    *
    *  The Player class describes all the attributes of
    *  a specific type of character in the application.
    */

    class Player : public Character {
    public:
        Player(int id, std::string username, std::string password);

        std::string getUsername() const;

        void setUsername(std::string username);

        std::string getPassword() const;

        void setPassword(std::string password);

        std::string getAvatar() const;

        void setAvatar(std::string avatar);

        bool operator==(const Player& player) const;

        /************ Inventory ************/

        bool isItemInInventory(Object item);

        bool isSlotOccupied(Slot slot);

        std::vector<Object> getInventoryItems() const;

        void mapInventoryItems(std::vector<Object> items);

        std::vector<Object> getEquippedItems() const;

        void mapEquippedItems(std::vector<Object> items);

        void equipItem(Object item);

        void addToInventoryItems(Object item);

        void unequipItem(Slot slot);

        Object dropItemFromInventory(Object item);

        Object dropItemFromEquipped(Slot slot);

    private:
        std::string username;

        std::string password;

        std::string avatar;

        std::map<int, Object> inventoryItems;

        std::map<int, Object> equippedItems;
    };
}

#endif //PLAYER_H