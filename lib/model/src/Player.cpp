//
// Created by jnhkm on 2019-01-18.
//

#include "Player.h"
#include <algorithm>
#include <sstream>

using model::Object;

namespace model {

    Player::Player() :
        Character({}),
        username({}),
        password({}),
        inventory({}),
        equipment({}),
        currRoomID(STARTING_LOCATION)
        {}


    Player::Player(model::ID id, std::string_view username, std::string_view password) :
        Character(id),
        username(username),
        password(password),
        inventory({}),
        equipment({}),
        currRoomID(STARTING_LOCATION)
        {}

    Player::Player(model::ID id, std::string_view username, std::string_view password, const model::ID &roomID) :
        Character(id),
        username(username),
        password(password),
        inventory({}),
        equipment({}),
        currRoomID(roomID)
        {}

    std::string Player::getUsername() const {
        return this->username;
    }

    void Player::setUsername(std::string_view username) {
        this->username = username;
    }

    std::string Player::getPassword() const {
        return this->password;
    }

    void Player::setPassword(std::string_view password) {
        this->password = password;
    }

    Avatar Player::getAvatar() const {
        return this->avatar;
    }

    std::string Player::getDescription() const {
        std::ostringstream description;
        auto avatar = this->getAvatar();
        auto equipment = this->equipment.getVectorEquipment();
        std::string genderPronoun;

        if (avatar.gender == model::Gender::Male) {
            genderPronoun = "He";

        } else if (avatar.gender == model::Gender::Female) {
            genderPronoun = "She";
        }

        description << this->getUsername() << " is a "
                     << avatar.trait1 << ", "
                     << avatar.trait2 << " "
                     << Avatar::genderToStringMap.at(avatar.gender) << " "
                     << Avatar::raceToStringMap.at(avatar.race);

        if (!equipment.empty()) {
            description << ". " << genderPronoun << " is equipped with " << equipment.at(0).getShortDescription();

            for (unsigned int i = 1; i < equipment.size(); ++i) {
                if (i == (equipment.size() - 1)) {
                    description << " and ";

                } else {
                    description << ", ";
                }

                description << equipment.at(i).getShortDescription();
            }
        }

        description << ".\n";
        return description.str();
    }

    void Player::setAvatar(Avatar avatar) {
        this->avatar = std::move(avatar);
    }

    model::ID Player::getCurrRoomID() {
        return this->currRoomID;
    }

    void Player::setCurrRoomID(const model::ID &id) {
        this->currRoomID = id;
    }

    Inventory &Player::getMutableInventory() {
        return this->inventory;
    }

    Inventory Player::getImmutableInventory() const{
        return this->inventory;
    }

    void Player::setInventory(const Inventory &inventory){
        this->inventory = inventory;
    }

    Equipment &Player::getMutableEquipment() {
        return this->equipment;
    }

    Equipment Player::getImmutableEquipment() const{
        return this->equipment;
    }

    void Player::setEquipment(const Equipment &equipment){
        this->equipment = equipment;
    }


    bool Player::operator==(const model::Player &player) const {
        return this->getId() == player.getId();
    }
}
