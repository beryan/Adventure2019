//
// Created by Stephen Wanhella on 2019-01-22.
//

#ifndef ACCOUNTHANDLER_H
#define ACCOUNTHANDLER_H

#include "Player.h"
#include "Server.h"
#include "json.hpp"

#include <map>
#include <deque>
#include <unordered_set>

using json = nlohmann::json;
using networking::Connection;
using networking::Message;
using model::Player;

namespace handler {

    class AccountHandler {
    private:
        static const unsigned short MIN_PASSWORD_LENGTH;
        static const unsigned short MAX_USERNAME_AND_PASSWORD_LENGTH;

        enum class RegisterStage{Username, Password, ConfirmPassword};
        enum class LoginStage{Username, Password};

        model::ID nextId;
        std::map<model::ID, Player> allPlayers;
        std::map<std::string, Player&> usernameToPlayer;
        std::map<model::ID, Connection> activeIdToClient;
        std::map<Connection, model::ID> activeClientToId;

        std::map<Connection, std::string> registerUsernameInput;
        std::map<Connection, std::string> registerPasswordInput;
        std::map<Connection, RegisterStage> clientRegisterStage;

        std::map<Connection, std::string> loginUsernameInput;
        std::map<Connection, LoginStage> clientLoginStage;

        std::vector<Connection> bootedClients;

    public:
        AccountHandler();

        static const model::ID INVALID_ID = 0;

        /**
         *  Checks if a player is logged in (exists in the activeClientToId map)
         */
        bool isLoggedIn(const Connection &client);

        /**
         *  Checks if a client is in the process of registering a new Player
         */
        bool isRegistering(const Connection &client);

        /**
         *  Processes and responds to the input of a registering user based on the step they are in
         */
        std::string processRegistration(const Connection &client, const std::string &input = "");

        /**
         *  Removes client from the registration process. Used for disconnects
         */
        void exitRegistration(const Connection &client);

        /**
         *  Checks if a client is in the process of logging in
         */
        bool isLoggingIn(const Connection &client);

        /**
         *  Processes and responds to the input of user logging in based on the step they are in. Appends bootedClients
         *  if logging into a Player that is already being accessed by another client.
         */
        std::string processLogin(const Connection &client, const std::string &input = "");

        /**
         *  Removes client from the login process. Used for disconnects
         */
        void exitLogin(const Connection &client);

        /**
         *  Logs out the client and returns a string to inform them.
         */
        std::string logoutClient(const Connection &client);

        /**
         *  Returns the username of a Player given a client ID. Used for displaying names in chat.
         */
        std::string getUsernameByClient(const Connection &client);

        /**
         *  Returns the client of a Player given a username
         */
        Connection getClientByUsername(const std::string &username);

        /**
         *  Returns the player ID of a Player given a client ID.
         */
        model::ID getPlayerIdByClient(const Connection &client);

        /**
         *  Returns a pointer to a Player given a client ID.
         */
        Player& getPlayerByClient(const Connection &client);

        /**
         *  Returns the room ID of a Player given a client ID.
         */
        model::ID getRoomIdByClient(const Connection &client);

        /**
         *  Sets the room ID of a Player given a client ID.
         */
        void setRoomIdByClient(const Connection &client, const model::ID &roomID);

        /**
         *  Returns the client ID of a Player given a player ID.
         */
        Connection getClientByPlayerId(const model::ID &playerId);

        /**
         *  Returns the username of a Player given a player ID
         */
        std::string getUsernameByPlayerId(const model::ID &playerId);

        /**
         *  Appends Responses based on clients who have been logged out due to a login by another client into the
         *  same Player. Is to be called by the Game class' handleOutgoing() method.
         */
        void notifyBootedClients(std::deque<Message> &messages);

        /**
         *  Swaps the clients of two active players. Returns true if successful.
         */
        void swapPlayerClientsByPlayerId(const model::ID &sourceId, const model::ID &targetId);

        void loadRegisteredUsers();

    };
}

#endif //ACCOUNTHANDLER_H
