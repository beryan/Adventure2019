//
// Created by Stephen Wanhella on 2019-01-22.
//

#include <iostream>
#include <sstream>

#include "PlayerHandler.h"
#include "Server.h"

using json = nlohmann::json;
using player = model::Player;
using networking::Connection;

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
    PlayerHandler::isLoggedIn(const Connection &client) {
        return (bool) activeClientToId.count(client);
    }

    bool
    PlayerHandler::isRegistering(const Connection &client) {
        return (bool) this->clientRegisterStage.count(client);
    }

    std::string
    PlayerHandler::processRegistration(const Connection &client, const std::string &input) {

        if (!this->clientRegisterStage.count(client)) {
            // Start registration process
            this->clientRegisterStage.emplace(client, RegisterStage::USERNAME);
            std::ostringstream response;
            response << "\n"
                     << "Register\n"
                     << "--------\n"
                     << "Enter your username (maximum of " << MAX_USERNAME_AND_PASSWORD_LENGTH << " characters)\n";

            return response.str();
        }

        switch (this->clientRegisterStage.at(client)) {
            case RegisterStage::USERNAME: {
                if (input.length() == 0) {
                    this->clientRegisterStage.erase(client);
                    return "No username entered. Registration process cancelled.\n";
                }

                if (input.length() > MAX_USERNAME_AND_PASSWORD_LENGTH) {
                    this->clientRegisterStage.erase(client);
                    return "The username you entered is too long. Registration process cancelled.\n";
                }

                if (this->usernameToPlayer.count(input)) {
                    return "The username \"" + input + "\" has already been taken, please use a different username.\n";
                }

                this->regUsernameInput.emplace(client, input);

                this->clientRegisterStage.at(client) = RegisterStage::PASSWORD;

                std::ostringstream response;
                response << input << "\n"
                         << "Enter your password (minimum of " << MIN_PASSWORD_LENGTH << " characters,"
                         << " maximum of " << MAX_USERNAME_AND_PASSWORD_LENGTH << " characters)\n";

                return response.str();
            }

            case RegisterStage::PASSWORD: {
                if (input.length() < MIN_PASSWORD_LENGTH) {
                    this->clientRegisterStage.erase(client);
                    return "The password you entered is too short. Registration process cancelled.\n";
                }

                if (input.length() > MAX_USERNAME_AND_PASSWORD_LENGTH) {
                    this->clientRegisterStage.erase(client);
                    return "The password you entered is too long. Registration process cancelled.\n";
                }

                this->regPasswordInput.emplace(client, input);
                this->clientRegisterStage.at(client) = RegisterStage::CONFIRM_PASSWORD;

                return "Re-enter your password\n";
            }

            case RegisterStage::CONFIRM_PASSWORD: {
                this->clientRegisterStage.erase(client);

                if (this->regPasswordInput.at(client) != input) {
                    return "The passwords you entered do not match. Registration process cancelled.\n";
                }

                auto inputUsername = this->regUsernameInput.at(client);
                auto inputPassword = this->regPasswordInput.at(client);
                this->regUsernameInput.erase(client);
                this->regPasswordInput.erase(client);

                if (this->usernameToPlayer.count(inputUsername)) {
                    return "The username \"" + inputUsername +
                           "\" has already been taken, please use a different username.\n";
                }

                auto playerId = this->nextId++;
                this->allPlayers.emplace(playerId, Player(playerId, inputUsername, inputPassword));
                this->usernameToPlayer.emplace(inputUsername, &this->allPlayers.at(playerId));

                this->activeClientToId.emplace(client, playerId);
                this->activeIdToClient.emplace(playerId, client);

                std::cout << inputUsername << " has been registered to the game\n";
                return "Your account has been successfully registered and you are now logged in.\n\n";
            }

            default: {
                throw "Undefined registration stage";
            }
        }
    }

    void
    PlayerHandler::exitRegistration(const Connection &client) {
        this->clientRegisterStage.erase(client);
        this->regUsernameInput.erase(client);
        this->regPasswordInput.erase(client);
    }

    bool
    PlayerHandler::isLoggingIn(const Connection &client) {
        return (bool) this->clientLoginStage.count(client);
    }

    std::string
    PlayerHandler::processLogin(const Connection &client, const std::string &input) {

        if (!this->clientLoginStage.count(client)) {
            // Start login process
            this->clientLoginStage.emplace(client, LoginStage::USERNAME);

            return "\n"
                   "Login\n"
                   "-----\n"
                   "Enter your username\n";
        }

        switch (this->clientLoginStage.at(client)) {
            case LoginStage::USERNAME: {
                if (input.length() == 0) {
                    this->clientLoginStage.erase(client);
                    return "No username entered. Login process cancelled.\n";
                }

                this->loginUsernameInput.emplace(client, input);
                this->clientLoginStage.at(client) = LoginStage::PASSWORD;

                return input + "\nEnter your password\n";
            }

            case LoginStage::PASSWORD: {
                this->clientLoginStage.erase(client);

                auto successMessage = "Logged in successfully!\n";
                auto failMessage = "Invalid username or password.\n";

                auto inputUsername = this->loginUsernameInput.at(client);
                this->loginUsernameInput.erase(client);

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

                    this->activeClientToId.emplace(client, playerId);
                    this->activeIdToClient.emplace(playerId, client);

                    std::cout << inputUsername << " is now being used by " << client.id << "\n";
                    return successMessage;

                } else {
                    this->activeClientToId.emplace(client, playerId);
                    this->activeIdToClient.emplace(playerId, client);

                    return successMessage;
                }
            }

            default:
                throw "Undefined login stage";
        }
    }

    void
    PlayerHandler::exitLogin(const Connection &client) {
        this->clientLoginStage.erase(client);
        this->loginUsernameInput.erase(client);
    }

    std::string
    PlayerHandler::logoutPlayer(const Connection &client) {
        this->activeIdToClient.erase(this->activeClientToId.at(client));
        this->activeClientToId.erase(client);

        return "Logged out successfully.\n";
    }

    std::string
    PlayerHandler::getUsernameByClient(const Connection &client) {
        return this->allPlayers.at(this->activeClientToId.at(client)).getUsername();
    }

    model::ID
    PlayerHandler::getPlayerIdByClient(const Connection &client) {
        return this->activeClientToId.at(client);
    }

    model::ID
    PlayerHandler::getRoomIdByClient(const Connection &client) {
        if (this->usernameToPlayer.count(this->getUsernameByClient(client)) > 0) {
            return this->usernameToPlayer.at(this->getUsernameByClient(client))->getCurrRoomID();
        }
        throw std::runtime_error("Error: usernameToPlayer map does not contain username");
    }

    void
    PlayerHandler::setRoomIdByClient(const Connection &client, const model::ID &roomID) {
        if (this->usernameToPlayer.count(this->getUsernameByClient(client)) > 0) {
            this->usernameToPlayer.at(this->getUsernameByClient(client))->setCurrRoomID(roomID);
        } else {
            throw std::runtime_error("Error: usernameToPlayer map does not contain username");
        }
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
            Player p (it.value().at("id"), it.value().at("username"), "");
            players.push_back(p);
        }

        return players;
    }
}
