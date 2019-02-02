//
// Created by jnhkm on 2019-01-18.
//

#include "Player.h"
#include <algorithm>

namespace model {
    Player::Player(model::ID id, std::string_view username, std::string_view password) :
        Character(id),
        username(std::move(username)),
        password(std::move(password)),
        inventoryItems({}),
        equippedItems({}),
        currRoomID(-1)
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

        for (auto const& [key, val] : inventoryItems)
            container.push_back(val);

        return container;
    }

    void Player::mapInventoryItems(std::vector<Object> &items) {
        for (Object item : items)
            inventoryItems.insert(std::pair<model::ID, Object>(item.getId(), std::move(item)));
    }

    std::vector<Object> Player::getEquippedItems() const {
        std::vector<Object> container;
        container.reserve(equippedItems.size());

        for (auto const& [key, val] : equippedItems)
            container.push_back(val);

        return container;
    }

    void Player::mapEquippedItems(std::vector<Object> &items) {
        for (Object item : items)
            equippedItems.insert(std::pair<int, Object>(item.getSlot(), std::move(item)));
    }

    void Player::equipItem(Object item) {
        if (item.canBeEquipped() && isItemInInventory(item)) {
            if (isSlotOccupied(item.getSlot())) {
                unequipItem(item.getSlot());
            }

            equippedItems.insert(std::pair<int, Object>(item.getSlot(), std::move(item)));
            inventoryItems.erase(item.getId());
        }
    }

    bool Player::isItemInInventory(const Object &item) {
        return inventoryItems.count(item.getId()) > 0;
    }

    bool Player::isSlotOccupied(const Slot &slot) {
        return equippedItems.count(slot) > 0;
    }

    void Player::addToInventoryItems(Object item) {
        inventoryItems.insert(std::pair<model::ID, Object>(item.getId(), std::move(item)));
    }

    void Player::unequipItem(const Slot &slot) {
        auto item = equippedItems.at(slot);
        equippedItems.erase(slot);

        inventoryItems.insert(std::pair<model::ID, Object>(item.getId(), item));
    }

    Object Player::dropItemFromInventory(Object item) {
        auto temp_item = inventoryItems.at(item.getId());
        inventoryItems.erase(item.getId());

        return std::move(temp_item);
    }

    Object Player::dropItemFromEquipped(Slot slot) {
        auto temp_item = equippedItems.at(slot);
        equippedItems.erase(slot);

        return std::move(temp_item);
    }

    model::ID Player::getCurrRoomID() {
        return this->currRoomID;
    }

    void Player::setCurrRoomID(const model::ID &id) {
        this->currRoomID = id;
    }

    bool Player::operator==(const model::Player &player) const {
        return this->getId() == player.getId();
    }
}