//
// Created by jnhkm on 2019-01-18.
//

#include "Player.h"

using model::Player;
using model::Role;

namespace model {
    std::string Player::getUsername() {return username;};

    void Player::setUsername(std::string username) {this->username.assign(username);};

    int Player::getPassword() {return password;};

    void Player::setPassword(int password) {this->password = password;};

    Role Player::getRole() {return role;};

    void Player::setRole(Role role) {this->role = role;};
}