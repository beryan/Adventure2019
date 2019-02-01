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
        Player(model::ID id, std::string_view username, std::string_view password);

        std::string getUsername() const;

        void setUsername(std::string_view username);

        std::string getPassword() const;

        void setPassword(std::string_view password);

        std::string getAvatar() const;

        void setAvatar(std::string_view avatar);

        bool operator==(const Player& player) const;

        /************ Inventory ************/

        bool isItemInInventory(const Object &item);

        bool isSlotOccupied(const Slot &slot);

        std::vector<Object> getInventoryItems() const;

        void mapInventoryItems(std::vector<Object> &items);

        std::vector<Object> getEquippedItems() const;

        void mapEquippedItems(std::vector<Object> &items);

        void equipItem(Object item);

        void addToInventoryItems(Object item);

        void unequipItem(const Slot &slot);

        Object dropItemFromInventory(Object item);

        Object dropItemFromEquipped(Slot slot);

        /************ ROOM ************/

        model::ID getCurrRoomID();

        void setCurrRoomId(const model::ID &id);

    private:
        std::string username;

        std::string password;

        std::string avatar;

        std::map<model::ID, Object> inventoryItems;

        std::map<int, Object> equippedItems;

        mode::currRoomID;
    };
}

#endif //PLAYER_H