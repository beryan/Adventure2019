//
// Created by Stephen Wanhella on 2019-01-22.
//

#include <PlayerHandler.h>
#include <iostream>
#include <sstream>

#include "PlayerHandler.h"

using json = nlohmann::json;
using player = model::Player;

namespace model {
    std::vector<Player> PlayerHandler::parseJsonUsers(json users) {
        std::vector<Player> players;

        for (json::iterator it = users.begin(); it != users.end(); ++it) {
          Player p (it.value().at("id"), it.value().at("username"));
          players.push_back(p);
        }

        return players;
    }

    PlayerHandler::PlayerHandler() {
        this->nextId = 1;
        this->allPlayers = {};
        this->usernameToPlayer = {};
        this->activeIdToClient = {};
        this->activeClientToId = {};
        this->bootedClients = {};
    }

    std::string
    PlayerHandler::registerPlayer(const uintptr_t &clientId, const std::string &param) {
        if (param.find(' ') == std::string::npos) {
            return "Missing fields for registration.\n";
        }

        auto inputUsername = param.substr(0, param.find(' '));
        auto inputPassword = param.substr(param.find(' ') + 1);

        if (this->usernameToPlayer.count(inputUsername)) {
          return "The username " + inputUsername + " has already been taken, please use a different username.\n";
        }

        auto playerId = this->nextId++;
        this->allPlayers.emplace(playerId, Player(playerId, inputUsername, inputPassword));
        this->usernameToPlayer.emplace(inputUsername, &this->allPlayers.at(playerId));

        this->activeClientToId.emplace(clientId, playerId);
        this->activeIdToClient.emplace(playerId, clientId);

        std::cout << inputUsername << " has been registered to the game\n";
        return "Your account has been successfully registered and you are now logged in.\n";
    }

    bool
    PlayerHandler::isLoggedIn(const uintptr_t &clientId) {
        return (bool) activeClientToId.count(clientId);
    }

    std::string
    PlayerHandler::loginPlayer(const uintptr_t &clientId, const std::string &param) {
        bool successfulLogin = false;

        if (param.find(' ') != std::string::npos) {
            auto inputUsername = param.substr(0, param.find(' '));
            auto inputPassword = param.substr(param.find(' ') + 1);

            if (this->usernameToPlayer.count(inputUsername)) {
                auto selectedPlayer = this->usernameToPlayer.at(inputUsername);
                auto playerId = selectedPlayer->getId();

                if ((selectedPlayer->getPassword() == inputPassword) && !this->activeIdToClient.count(playerId)) {
                    this->activeClientToId.emplace(clientId, playerId);
                    this->activeIdToClient.emplace(playerId, clientId);

                } else {
                    // Player is already being used by a client, logout associated client
                    // and login with new client
                    auto otherClientId = this->activeIdToClient.at(playerId);
                    this->bootedClients.push_back(otherClientId);

                    this->activeClientToId.erase(otherClientId);
                    this->activeIdToClient.erase(playerId);

                    this->activeClientToId.emplace(clientId, playerId);
                    this->activeIdToClient.emplace(playerId, clientId);

                    std::cout << inputUsername << " is now being used by " << clientId << "\n";

                }

                successfulLogin = true;
            }
        }

      if (successfulLogin) {
          return "Logged in successfully!\n";

      } else {
          return "Invalid username or password.\n";
      }
    }

    std::string
    PlayerHandler::logoutPlayer(const uintptr_t &clientId) {
        this->activeIdToClient.erase(this->activeClientToId.at(clientId));
        this->activeClientToId.erase(clientId);

        return "Logged out successfully.\n";
    }

    std::string
    PlayerHandler::getUsernameByClientId(const uintptr_t &clientId) {
        return this->allPlayers.at(this->activeClientToId.at(clientId)).getUsername();
    }

    void
    PlayerHandler::notifyBootedClients(std::deque<Response> &responses) {
        auto itr = this->bootedClients.begin();

        while (itr != this->bootedClients.end()) {
            auto clientId = *itr;
            responses.push_back({clientId, "You have been logged out due to being logged in elsewhere.\n", true});

            this->bootedClients.erase(itr);
        }
    }
}
