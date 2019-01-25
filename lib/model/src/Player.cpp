//
// Created by jnhkm on 2019-01-18.
//

#include "Player.h"


namespace model {

    bool isItemInInventoryItems(const std::vector<Object> &inventoryItems, Object *item);

    void addItemToEquippedItems(Object *currentItemInSlot, Object *newItemToAdd);

    bool isSlotOccupied(Object *currentItemInSlot);

    Player::Player(int id, std::string username, std::string password) :
        Character(id),
        username(std::move(username)),
        password(std::move(password))
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

    Role Player::getRole() const {
        return this->role;
    }

    void Player::setRole(Role role) {
        this->role = role;
    }

    std::string Player::getAvatar() const {
        return this->avatar;
    }

    void Player::setAvatar(std::string avatar) {
        this->avatar = std::move(avatar);
    }

    std::vector<Object> Player::getInventoryItems() const {
        return this->inventoryItems;
    }

    void Player::setInventoryItems(std::vector<Object> inventoryItems) {
        this->inventoryItems = std::move(inventoryItems);
    }

    void Player::addToInventoryItems(Object addToInventoryItems) {
        this->inventoryItems.push_back(std::move(addToInventoryItems));
    }

    std::array<Object *, Slot::Count> Player::getEquippedItems() const {
        return this->equippedItems;
    }

    void Player::setEquippedItems(std::array<Object*, Slot::Count> equippedItems) {
        this->equippedItems = equippedItems;
    }

    void Player::moveFromInventoryToEquippedItems(Object *itemToMove) {
        if (itemToMove->canBeEquipped() &&
            isItemInInventoryItems(this->inventoryItems, itemToMove)) {

            Object* currentItemInSlot = this->equippedItems.at(itemToMove->getSlot());

            addItemToEquippedItems(currentItemInSlot, itemToMove);

            removeFromInventoryItems(itemToMove);
        }
    }

    bool isItemInInventoryItems(const std::vector<Object> &inventoryItems, Object *item) {
        return (std::find(inventoryItems.begin(), inventoryItems.end(), *item) != inventoryItems.end());
    }

    void addItemToEquippedItems(Object *currentItemInSlot, Object *newItemToAdd) {
        if (isSlotOccupied(currentItemInSlot)) {
            std::swap(*currentItemInSlot, *newItemToAdd);
        } else {
            *currentItemInSlot = std::move(*newItemToAdd);
        }
    }

    bool isSlotOccupied(Object *currentItemInSlot) {
        return currentItemInSlot != nullptr;
    }

    void Player::moveFromEquippedToInventoryItems(Slot slotToMoveFrom) {
        Object *itemToMove = removeFromEquippedItems(slotToMoveFrom);
        if (isSlotOccupied(itemToMove)) {
            this->inventoryItems.push_back(std::move(*itemToMove));
        }
    }

    Object* Player::removeFromInventoryItems(Object *itemToRemove) {
        this->inventoryItems.erase(
            std::find(this->inventoryItems.begin(), this->inventoryItems.end(), *itemToRemove)
        );
        return itemToRemove;
    }

    Object* Player::removeFromEquippedItems(Slot slotToRemoveFrom) {
        Object *itemToRemove = this->equippedItems.at(slotToRemoveFrom);
        this->equippedItems.at(slotToRemoveFrom) = nullptr;
        return itemToRemove;
    }

    bool Player::operator==(const model::Player &player) const {
        return this->getId() == player.getId();
    }
}