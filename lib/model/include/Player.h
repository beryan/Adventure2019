//
// Created by jnhkm on 2019-01-18.
//

#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <array>
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

        Player(int id, std::string username);

        std::string getUsername() const;

        void setUsername(std::string username);

        std::string getPassword() const;

        void setPassword(std::string password);

        std::string getAvatar() const;

        void setAvatar(std::string avatar);

        bool operator==(const Player& player) const;

        /************ Inventory ************/

        std::vector<Object> getInventoryItems() const;

        void setInventoryItems(std::vector<Object> inventoryItems);

        std::array<Object*, Slot::Count> getEquippedItems() const;

        void setEquippedItems(std::array<Object*, Slot::Count> equippedItems);

        void moveFromInventoryToEquippedItems(Object *itemToMove);

        bool isItemInInventoryItems(const std::vector<Object> &inventoryItems, Object *item);

        void addItemToEquippedItems(Object *currentItemInSlot, Object *newItemToAdd);

        bool isSlotOccupied(Object *currentItemInSlot);

        void addToInventoryItems(Object addToInventoryItems);

        void moveFromEquippedToInventoryItems(Slot slotToMoveFrom);

        Object* dropFromInventoryItems(Object *itemToRemove);

        Object* dropFromEquippedItems(Slot slotToRemoveFrom);

    private:
        std::string username;

        std::string password;

        std::string avatar;

        std::map<int, Object> inventoryItems;

        std::map<int, Object> equippedItems;
    };
}

#endif //PLAYER_H