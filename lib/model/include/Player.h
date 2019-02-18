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
        Player();

        Player(model::ID id, std::string_view username, std::string_view password);

        Player(model::ID id, std::string_view username, std::string_view password, const model::ID &roomID);

        std::string getUsername() const;

        void setUsername(std::string_view username);

        std::string getPassword() const;

        void setPassword(std::string_view password);

        std::string getAvatar() const;

        void setAvatar(std::string_view avatar);

        bool operator==(const Player& player) const;

        /************ Inventory ************/

        std::map<model::ID, Object> getMappedInventoryItems();

        std::vector<Object> getInventoryItems() const;

        void mapInventoryItems(std::vector<Object> &items);

        std::map<int, Object> getMappedEquippedItems();

        std::vector<Object> getEquippedItems() const;

        void mapEquippedItems(std::vector<Object> &items);

        void addToInventoryItems(Object object);

        void addToEquippedItems(Object object);

        Object removeInventoryItem(Object object);

        Object removeEquippedItem(Slot slot);

        /************ ROOM ************/

        model::ID getCurrRoomID();

        void setCurrRoomID(const model::ID &id);

        static constexpr model::ID STARTING_LOCATION = 8810;

    private:
        std::string username;

        std::string password;

        std::string avatar;

        std::map<model::ID, Object> inventoryItems;

        std::map<int, Object> equippedItems;

        model::ID currRoomID;
    };

    inline void from_json(const json &j, Player &p) {
        p.setUsername(j.at("username").get<std::string>());
        p.setPassword(j.at("password").get<std::string>());
    }
}

#endif //PLAYER_H
