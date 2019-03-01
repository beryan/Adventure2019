//
// Created by Stephen Wanhella on 2019-01-22.
//

#include <iostream>
#include <sstream>

#include "AccountHandler.h"
#include "Server.h"

using model::AccountHandler;
using json = nlohmann::json;
using networking::Connection;

namespace model {
    const unsigned short AccountHandler::MIN_PASSWORD_LENGTH = 4;
    const unsigned short AccountHandler::MAX_USERNAME_AND_PASSWORD_LENGTH = 16;

    AccountHandler::AccountHandler() {
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
    AccountHandler::isLoggedIn(const Connection &client) {
        return static_cast<bool> (activeClientToId.count(client));
    }


    bool
    AccountHandler::isRegistering(const Connection &client) {
        return static_cast<bool> (this->clientRegisterStage.count(client));
    }


    std::string
    AccountHandler::processRegistration(const Connection &client, const std::string &input) {

        if (!this->clientRegisterStage.count(client)) {
            // Start registration process
            this->clientRegisterStage.emplace(client, RegisterStage::Username);
            std::ostringstream response;
            response << "\n"
                     << "Register\n"
                     << "--------\n"
                     << "Enter your username (maximum of " << MAX_USERNAME_AND_PASSWORD_LENGTH << " characters)\n";

            return response.str();
        }

        switch (this->clientRegisterStage.at(client)) {
            case RegisterStage::Username: {
                if (input.length() == 0) {
                    this->exitRegistration(client);
                    return "No username entered. Registration process cancelled.\n";
                }

                if (input.length() > MAX_USERNAME_AND_PASSWORD_LENGTH) {
                    this->exitRegistration(client);
                    return "The username you entered is too long. Registration process cancelled.\n";
                }

                if (this->usernameToPlayer.count(input)) {
                    return "The username \"" + input + "\" has already been taken, please use a different username.\n";
                }

                this->regUsernameInput.emplace(client, input);

                this->clientRegisterStage.at(client) = RegisterStage::Password;

                std::ostringstream response;
                response << input << "\n"
                         << "Enter your password (minimum of " << MIN_PASSWORD_LENGTH << " characters,"
                         << " maximum of " << MAX_USERNAME_AND_PASSWORD_LENGTH << " characters)\n";

                return response.str();
            }

            case RegisterStage::Password: {
                if (input.length() < MIN_PASSWORD_LENGTH) {
                    this->exitRegistration(client);
                    return "The password you entered is too short. Registration process cancelled.\n";
                }

                if (input.length() > MAX_USERNAME_AND_PASSWORD_LENGTH) {
                    this->exitRegistration(client);
                    return "The password you entered is too long. Registration process cancelled.\n";
                }

                this->regPasswordInput.emplace(client, input);
                this->clientRegisterStage.at(client) = RegisterStage::ConfirmPassword;

                return "Re-enter your password\n";
            }

            case RegisterStage::ConfirmPassword: {
                this->clientRegisterStage.erase(client);

                if (this->regPasswordInput.at(client) != input) {
                    this->exitRegistration(client);
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
                return "Your account has been successfully registered and you are now logged in.\n";
            }

            default: {
                throw "Undefined registration stage";
            }
        }
    }


    void
    AccountHandler::exitRegistration(const Connection &client) {
        this->clientRegisterStage.erase(client);
        this->regUsernameInput.erase(client);
        this->regPasswordInput.erase(client);
    }


    bool
    AccountHandler::isLoggingIn(const Connection &client) {
        return static_cast<bool> (this->clientLoginStage.count(client));
    }


    std::string
    AccountHandler::processLogin(const Connection &client, const std::string &input) {

        if (!this->clientLoginStage.count(client)) {
            // Start login process
            this->clientLoginStage.emplace(client, LoginStage::Username);

            return "\n"
                   "Login\n"
                   "-----\n"
                   "Enter your username\n";
        }

        switch (this->clientLoginStage.at(client)) {
            case LoginStage::Username: {
                if (input.length() == 0) {
                    this->exitLogin(client);
                    return "No username entered. Login process cancelled.\n";
                }

                this->loginUsernameInput.emplace(client, input);
                this->clientLoginStage.at(client) = LoginStage::Password;

                return input + "\nEnter your password\n";
            }

            case LoginStage::Password: {
                auto successMessage = "Logged in successfully!\n";
                auto failMessage = "Invalid username or password.\n";

                auto inputUsername = this->loginUsernameInput.at(client);
                this->loginUsernameInput.erase(client);

                auto playerExists = this->usernameToPlayer.count(inputUsername);

                if (!playerExists) {
                    this->exitLogin(client);
                    return failMessage;
                }

                auto passwordMatches = (this->usernameToPlayer.at(inputUsername)->getPassword() == input);

                if (!passwordMatches) {
                    this->exitLogin(client);
                    return failMessage;
                }

                auto playerId = this->usernameToPlayer.at(inputUsername)->getId();
                auto isOnline = static_cast<bool> (this->activeIdToClient.count(playerId));

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

                    this->exitLogin(client);
                    return successMessage;

                } else {
                    this->activeClientToId.emplace(client, playerId);
                    this->activeIdToClient.emplace(playerId, client);

                    this->exitLogin(client);
                    return successMessage;
                }
            }

            default:
                throw "Undefined login stage";
        }
    }


    void
    AccountHandler::exitLogin(const Connection &client) {
        this->clientLoginStage.erase(client);
        this->loginUsernameInput.erase(client);
    }


    std::string
    AccountHandler::logoutClient(const Connection &client) {
        this->activeIdToClient.erase(this->activeClientToId.at(client));
        this->activeClientToId.erase(client);

        return "Logged out successfully.\n";
    }


    std::string
    AccountHandler::getUsernameByClient(const Connection &client) {
        std::string username;
        auto it = this->allPlayers.find(getPlayerIdByClient(client));
        if (it != this->allPlayers.end()) {
            username = it->second.getUsername();
        }
        return username;
    }

    Connection
    AccountHandler::getClientByUsername(const std::string &username) {
        if (!this->usernameToPlayer.count(username)) {
            return {0};
        }

        auto player = this->usernameToPlayer.at(username);

        if (!this->activeIdToClient.count(player->getId())) {
            return {0};
        }

        return this->activeIdToClient.at(player->getId());
    }

    model::ID
    AccountHandler::getPlayerIdByClient(const Connection &client) {
        model::ID result = -1;
        auto it = this->activeClientToId.find(client);
        if (it != this->activeClientToId.end()) {
            result = it->second;
        }
        return result;
    }


    model::ID
    AccountHandler::getRoomIdByClient(const Connection &client) {
        if (this->usernameToPlayer.count(this->getUsernameByClient(client)) > 0) {
            return this->usernameToPlayer.at(this->getUsernameByClient(client))->getCurrRoomID();
        }

        return this->usernameToPlayer.at(this->getUsernameByClient(client))->getCurrRoomID();
    }


    void
    AccountHandler::setRoomIdByClient(const Connection &client, const model::ID &roomID) {
        if (this->usernameToPlayer.count(this->getUsernameByClient(client)) > 0) {
            this->usernameToPlayer.at(this->getUsernameByClient(client))->setCurrRoomID(roomID);
        } else {
            throw std::runtime_error("Error: usernameToPlayer map does not contain username");
        }

        this->usernameToPlayer.at(this->getUsernameByClient(client))->setCurrRoomID(roomID);
    }


    Connection
    AccountHandler::getClientByPlayerId(const model::ID &playerId) {
        return this->activeIdToClient.at(playerId);
    }


    void
    AccountHandler::notifyBootedClients(std::deque<Message> &messages) {
        for (auto bootedClient : this->bootedClients) {
            messages.push_back({bootedClient, "You have been logged out due to being logged in elsewhere.\n"});
        }

        this->bootedClients.clear();
    }

    bool
    AccountHandler::swapPlayerClientsByPlayerId(const ID &sourceId, const ID &targetId) {
        try {
            auto sourceClient = this->activeIdToClient.at(sourceId);
            auto targetClient = this->activeIdToClient.at(targetId);

            auto sourceUsername = this->getUsernameByClient(sourceClient);
            auto targetUsername = this->getUsernameByClient(targetClient);

            this->activeIdToClient.at(sourceId) = targetClient;
            this->activeClientToId.at(sourceClient) = targetId;

            this->activeIdToClient.at(targetId) = sourceClient;
            this->activeClientToId.at(targetClient) = sourceId;


            std::cout << sourceClient.id << " (now " << targetUsername << ")"
                      <<" swapped Players with "
                      << targetClient.id << " (now " << sourceUsername << ")"
                      << "\n";

            return true;

        } catch(const std::out_of_range &error) {
            return false;
        }
    }

    std::vector<Player>
    AccountHandler::parseJsonUsers(json users) {
        std::vector<Player> players;

        for (json::iterator it = users.begin(); it != users.end(); ++it) {
            Player p (it.value().at("id"), it.value().at("username"), "");
            players.push_back(p);
        }

        return players;
    }
}
