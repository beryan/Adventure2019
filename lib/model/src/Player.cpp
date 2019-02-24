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
            inventory({}),
            equipments({}),
            currRoomID(STARTING_LOCATION)
            {}


    Player::Player(model::ID id, std::string_view username, std::string_view password) :
        Character(id),
        username(std::move(username)),
        password(std::move(password)),
        inventory({}),
        equipments({}),
        currRoomID(STARTING_LOCATION)
        {}

    Player::Player(model::ID id, std::string_view username, std::string_view password, const model::ID &roomID) :
        Character(id),
        username(std::move(username)),
        password(std::move(password)),
        inventory({}),
        equipments({}),
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

    model::ID Player::getCurrRoomID() {
        return this->currRoomID;
    }

    void Player::setCurrRoomID(const model::ID &id) {
        this->currRoomID = id;
    }

    Inventory Player::getInventory() {
        return inventory;
    }

    Equipment Player::getEquipments() {
        return equipments;
    }

    bool Player::operator==(const model::Player &player) const {
        return this->getId() == player.getId();
    }
}
