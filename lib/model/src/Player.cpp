//
// Created by jnhkm on 2019-01-18.
//

#include "Player.h"
#include <algorithm>

namespace model {
    Player::Player(int id, std::string username, std::string password) :
        Character(id),
        username(std::move(username)),
        password(std::move(password))
        {}

    Player::Player(int id, std::string username) :
        Character(id),
        username(std::move(username)),
        password("foobar")
        {}

    std::string Player::getUsername() const {
        return this->username;
    }

    void Player::setUsername(std::string username) {
        this->username = std::move(username);
    }

    std::string Player::getPassword() const {
        return password;
    }

    void Player::setPassword(std::string password) {
        this->password = std::move(password);
    }

    std::string Player::getAvatar() const {
        return this->avatar;
    }

    void Player::setAvatar(std::string avatar) {
        this->avatar = std::move(avatar);
    }

    std::vector<Object> Player::getInventoryItems() const {
        std::vector<Object> container(inventoryItems.size());

        for (auto const& [key, val] : inventoryItems)
            container.push_back(val);

        return container;
    }

    void Player::mapInventoryItems(std::vector<Object> items) {
        for (Object item : items)
            inventoryItems.insert(std::pair<int, Object>(item.getId(), std::move(item)));
    }

    std::vector<Object> Player::getEquippedItems() const {
        std::vector<Object> container(equippedItems.size());

        for (auto const& [key, val] : equippedItems)
            container.push_back(val);

        return container;
    }

    void Player::mapEquippedItems(std::vector<Object> items) {
        for (Object item : items)
            equippedItems.insert(std::pair<int, Object>(item.getSlot(), std::move(item)));
    }

    void Player::equipItem(Object item) {
        if (item.canBeEquipped() && isItemInInventory(item)) {
            unequipItem(item.getSlot());

            equippedItems.insert(std::pair<int, Object>(item.getSlot(), std::move(item)));
        }
    }

    bool Player::isItemInInventory(Object item) {
        return inventoryItems.count(item.getId()) > 0;
    }

    bool Player::isSlotOccupied(Slot slot) {
        return equippedItems.count(slot) > 0;
    }

    void Player::addToInventoryItems(Object item) {
        inventoryItems.insert(std::pair<int, Object>(item.getId(), std::move(item)));
    }

    void Player::unequipItem(Slot slot) {
        Object *temp_item = nullptr;

        if (isSlotOccupied(slot)) {
            *temp_item = equippedItems.at(slot);
            equippedItems.erase(slot);
        }

        if (temp_item != nullptr)
            inventoryItems.insert(std::pair<int, Object>(temp_item->getId(), *temp_item));
    }

    Object* Player::dropItemFromInventory(Object item) {
        Object *temp_item = nullptr;

        if (isItemInInventory(item)) {
            *temp_item = inventoryItems.at(item.getId());
            inventoryItems.erase(item.getId());
        }

        return temp_item;
    }

    Object* Player::dropItemFromEquipped(Slot slot) {
        Object *temp_item = nullptr;

        if (isSlotOccupied(slot)) {
            *temp_item = equippedItems.at(slot);
            equippedItems.erase(slot);
        }

        return temp_item;
    }

    bool Player::operator==(const model::Player &player) const {
        return this->getId() == player.getId();
    }
}