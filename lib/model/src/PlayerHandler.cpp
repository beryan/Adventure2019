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
    const unsigned short PlayerHandler::MIN_PASSWORD_LENGTH = 4;
    const unsigned short PlayerHandler::MAX_USERNAME_AND_PASSWORD_LENGTH = 16;

    PlayerHandler::PlayerHandler() {
        this->nextId = 1;
        this->allPlayers = {};
        this->usernameToPlayer = {};
        this->activeIdToClient = {};
        this->activeClientToId = {};

        this->clientRegisterStage = {};
        this->regUsernameInput = {};
        this->regPasswordInput = {};

        this->clientLoginStage= {};
        this->loginUsernameInput = {};

        this->bootedClients = {};
    }

    bool
    PlayerHandler::isLoggedIn(const uintptr_t &clientId) {
        return (bool) activeClientToId.count(clientId);
    }

    bool
    PlayerHandler::isRegistering(const uintptr_t &clientId) {
        return (bool) this->clientRegisterStage.count(clientId);
    }

    std::string
    PlayerHandler::processRegistration(const uintptr_t &clientId, const std::string &input) {

        if (!this->clientRegisterStage.count(clientId)) {
            // Start registration process
            this->clientRegisterStage.emplace(clientId, RegisterStage::USERNAME);
            std::ostringstream response;
            response << "\n"
                     << "Register\n"
                     << "--------\n"
                     << "Enter your username (maximum of " << MAX_USERNAME_AND_PASSWORD_LENGTH << " characters)\n";

            return response.str();
        }

        switch (this->clientRegisterStage.at(clientId)) {
            case RegisterStage::USERNAME: {
                if (input.length() == 0) {
                    this->clientRegisterStage.erase(clientId);
                    return "No username entered. Registration process cancelled.\n";
                }

                if (input.length() > MAX_USERNAME_AND_PASSWORD_LENGTH) {
                    this->clientRegisterStage.erase(clientId);
                    return "The username you entered is too long. Registration process cancelled.\n";
                }

                if (this->usernameToPlayer.count(input)) {
                    return "The username \"" + input + "\" has already been taken, please use a different username.\n";
                }

                this->regUsernameInput.emplace(clientId, input);

                this->clientRegisterStage.at(clientId) = RegisterStage::PASSWORD;

                std::ostringstream response;
                response << input << "\n"
                         << "Enter your password (minimum of " << MIN_PASSWORD_LENGTH << " characters,"
                         << " maximum of " << MAX_USERNAME_AND_PASSWORD_LENGTH << " characters)\n";

                return response.str();
            }

            case RegisterStage::PASSWORD: {
                if (input.length() < MIN_PASSWORD_LENGTH) {
                    this->clientRegisterStage.erase(clientId);
                    return "The password you entered is too short. Registration process cancelled.\n";
                }

                if (input.length() > MAX_USERNAME_AND_PASSWORD_LENGTH) {
                    this->clientRegisterStage.erase(clientId);
                    return "The password you entered is too long. Registration process cancelled.\n";
                }

                this->regPasswordInput.emplace(clientId, input);
                this->clientRegisterStage.at(clientId) = RegisterStage::CONFIRM_PASSWORD;

                return "Re-enter your password\n";
            }

            case RegisterStage::CONFIRM_PASSWORD: {
                this->clientRegisterStage.erase(clientId);

                if (this->regPasswordInput.at(clientId) != input) {
                    return "The passwords you entered do not match. Registration process cancelled.\n";
                }

                auto inputUsername = this->regUsernameInput.at(clientId);
                auto inputPassword = this->regPasswordInput.at(clientId);
                this->regUsernameInput.erase(clientId);
                this->regPasswordInput.erase(clientId);

                if (this->usernameToPlayer.count(inputUsername)) {
                    return "The username \"" + inputUsername +
                           "\" has already been taken, please use a different username.\n";
                }

                auto playerId = this->nextId++;
                this->allPlayers.emplace(playerId, Player(playerId, inputUsername, inputPassword));
                this->usernameToPlayer.emplace(inputUsername, &this->allPlayers.at(playerId));

                this->activeClientToId.emplace(clientId, playerId);
                this->activeIdToClient.emplace(playerId, clientId);

                std::cout << inputUsername << " has been registered to the game\n";
                return "Your account has been successfully registered and you are now logged in.\n\n";
            }

            default: {
                throw "Undefined registration stage";
            }
        }
    }

    void
    PlayerHandler::exitRegistration(const uintptr_t &clientId) {
        this->clientRegisterStage.erase(clientId);
        this->regUsernameInput.erase(clientId);
        this->regPasswordInput.erase(clientId);
    }

    bool
    PlayerHandler::isLoggingIn(const uintptr_t &clientId) {
        return (bool) this->clientLoginStage.count(clientId);
    }

    std::string
    PlayerHandler::processLogin(const uintptr_t &clientId, const std::string &input) {

        if (!this->clientLoginStage.count(clientId)) {
            // Start login process
            this->clientLoginStage.emplace(clientId, LoginStage::USERNAME);

            return "\n"
                   "Login\n"
                   "-----\n"
                   "Enter your username\n";
        }

        switch (this->clientLoginStage.at(clientId)) {
            case LoginStage::USERNAME: {
                if (input.length() == 0) {
                    this->clientLoginStage.erase(clientId);
                    return "No username entered. Login process cancelled.\n";
                }

                this->loginUsernameInput.emplace(clientId, input);
                this->clientLoginStage.at(clientId) = LoginStage::PASSWORD;

                return input + "\nEnter your password\n";
            }

            case LoginStage::PASSWORD: {
                this->clientLoginStage.erase(clientId);

                auto successMessage = "Logged in successfully!\n";
                auto failMessage = "Invalid username or password.\n";

                auto inputUsername = this->loginUsernameInput.at(clientId);
                this->loginUsernameInput.erase(clientId);

                auto playerExists = this->usernameToPlayer.count(inputUsername);

                if (!playerExists) {
                    return failMessage;
                }

                auto passwordMatches = (this->usernameToPlayer.at(inputUsername)->getPassword() == input);

                if (!passwordMatches) {
                    return failMessage;
                }

                auto playerId = this->usernameToPlayer.at(inputUsername)->getId();
                auto isOnline = (bool) this->activeIdToClient.count(playerId);

                if (isOnline) {
                    // Player is already being used by a client, logout associated client
                    // and login with new client
                    auto otherClientId = this->activeIdToClient.at(playerId);
                    this->bootedClients.push_back(otherClientId);

                    this->activeClientToId.erase(otherClientId);
                    this->activeIdToClient.erase(playerId);

                    this->activeClientToId.emplace(clientId, playerId);
                    this->activeIdToClient.emplace(playerId, clientId);

                    std::cout << inputUsername << " is now being used by " << clientId << "\n";
                    return successMessage;

                } else {
                    this->activeClientToId.emplace(clientId, playerId);
                    this->activeIdToClient.emplace(playerId, clientId);

                    return successMessage;
                }
            }

            default:
                throw "Undefined login stage";
        }
    }

    void
    PlayerHandler::exitLogin(const uintptr_t &clientId) {
        this->clientLoginStage.erase(clientId);
        this->loginUsernameInput.erase(clientId);
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
        for (auto bootedClient : this->bootedClients) {
            responses.push_back({bootedClient, "You have been logged out due to being logged in elsewhere.\n", true});
        }

        this->bootedClients.clear();
    }

    std::vector<Player>
    PlayerHandler::parseJsonUsers(json users) {
        std::vector<Player> players;

        for (json::iterator it = users.begin(); it != users.end(); ++it) {
            Player p (it.value().at("id"), it.value().at("username"));
            players.push_back(p);
        }

        return players;
    }
}
