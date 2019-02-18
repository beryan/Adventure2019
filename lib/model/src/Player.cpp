//
// Created by jnhkm on 2019-01-18.
//

#include "Player.h"
#include <algorithm>

using model::Object;

namespace model {

    Player::Player() :
            Character({}),
            username({}),
            password({}),
            inventoryItems({}),
            equippedItems({}),
            currRoomID(STARTING_LOCATION)
            {}


    Player::Player(model::ID id, std::string_view username, std::string_view password) :
        Character(id),
        username(std::move(username)),
        password(std::move(password)),
        inventoryItems({}),
        equippedItems({}),
        currRoomID(STARTING_LOCATION)
        {}

    Player::Player(model::ID id, std::string_view username, std::string_view password, const model::ID &roomID) :
        Character(id),
        username(std::move(username)),
        password(std::move(password)),
        inventoryItems({}),
        equippedItems({}),
        currRoomID(roomID)
        {}

    std::string Player::getUsername() const {
        return this->username;
    }

    void Player::setUsername(std::string_view username) {
        this->username = std::move(username);
    }

    std::string Player::getPassword() const {
        return password;
    }

    void Player::setPassword(std::string_view password) {
        this->password = std::move(password);
    }

    std::string Player::getAvatar() const {
        return this->avatar;
    }

    void Player::setAvatar(std::string_view avatar) {
        this->avatar = std::move(avatar);
    }

    std::vector<Object> Player::getInventoryItems() const {
        std::vector<Object> container;
        container.reserve(inventoryItems.size());

        for (auto const& [key, val] : inventoryItems) {
            container.push_back(val);
        }

        return container;
    }

    std::map<model::ID, Object> Player::getMappedInventoryItems() {
        return this->inventoryItems;
    }

    std::map<int, Object> Player::getMappedEquippedItems() {
        return this->equippedItems;
    }

    void Player::mapInventoryItems(std::vector<Object> &items) {
        for (Object& item : items) {
            inventoryItems.insert(std::pair<model::ID, Object>(item.getId(), item));
        }
    }

    std::vector<Object> Player::getEquippedItems() const {
        std::vector<Object> container;
        container.reserve(equippedItems.size());

        for (auto const& [key, val] : equippedItems) {
            container.push_back(val);
        }

        return container;
    }

    void Player::mapEquippedItems(std::vector<Object> &items) {
        for (Object& item : items) {
            equippedItems.insert(std::pair<int, Object>(item.getSlot(), item));
        }
    }

    model::ID Player::getCurrRoomID() {
        return this->currRoomID;
    }

    void Player::setCurrRoomID(const model::ID &id) {
        this->currRoomID = id;
    }

    void Player::addToInventoryItems(Object object) {
        this->inventoryItems.insert(std::pair<model::ID, Object>(object.getId(), std::move(object)));
    }

    void Player::addToEquippedItems(Object object) {
        this->equippedItems.insert(std::pair<int, Object>(object.getSlot(), std::move(object)));
    }

    Object Player::removeInventoryItem(Object object) {
        Object temp = std::move(this->inventoryItems.at(object.getId()));
        this->inventoryItems.erase(object.getId());

        return std::move(temp);
    }

    Object Player::removeEquippedItem(Slot slot) {
        Object temp = std::move(this->equippedItems.at(slot));
        this->equippedItems.erase(slot);

        return std::move(temp);
    }

    bool Player::operator==(const model::Player &player) const {
        return this->getId() == player.getId();
    }
}
