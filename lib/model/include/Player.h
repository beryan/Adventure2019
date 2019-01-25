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

        std::string getUsername() const;

        void setUsername(std::string username);

        std::string getPassword() const;

        void setPassword(std::string password);

        std::string getAvatar() const;

        void setAvatar(std::string avatar);

        std::vector<Object> getInventoryItems() const;

        void setInventoryItems(std::vector<Object> inventoryItems);

        void addToInventoryItems(Object addToInventoryItems);

        std::array<Object*, Slot::Count> getEquippedItems() const;

        void setEquippedItems(std::array<Object*, Slot::Count> equippedItems);

        void moveFromInventoryToEquippedItems(Object *itemToMove);

        void moveFromEquippedToInventoryItems(Slot slotToMoveFrom);

        Object* removeFromInventoryItems(Object *itemToRemove);

        Object* removeFromEquippedItems(Slot slotToRemoveFrom);

        bool operator==(const Player& player) const;

    private:
        std::string username;

        std::string password;

        std::string avatar;

        std::vector<Object> inventoryItems;

        std::array<Object*, Slot::Count> equippedItems;
    };
}

#endif //PLAYER_H