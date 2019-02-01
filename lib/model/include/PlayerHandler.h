//
// Created by Stephen Wanhella on 2019-01-22.
//

#ifndef PLAYERHANDLER_H
#define PLAYERHANDLER_H

#include "Player.h"
#include "Response.h"
#include "Server.h"
#include "json.hpp"

#include <map>
#include <deque>
#include <unordered_set>

using json = nlohmann::json;
using networking::Connection;

namespace model {

    class PlayerHandler {
    private:
        static const unsigned short MIN_PASSWORD_LENGTH;
        static const unsigned short MAX_USERNAME_AND_PASSWORD_LENGTH;

        enum class RegisterStage{USERNAME, PASSWORD, CONFIRM_PASSWORD};
        enum class LoginStage{USERNAME, PASSWORD};

        model::ID nextId;
        std::map<model::ID, Player> allPlayers;
        std::map<std::string, Player*> usernameToPlayer;
        std::map<model::ID, Connection> activeIdToClient;
        std::map<Connection, model::ID> activeClientToId;

        std::map<Connection, std::string> regUsernameInput;
        std::map<Connection, std::string> regPasswordInput;
        std::map<Connection, RegisterStage> clientRegisterStage;

        std::map<Connection, std::string> loginUsernameInput;
        std::map<Connection, LoginStage> clientLoginStage;

        std::vector<Connection> bootedClients;

    public:
        PlayerHandler();

        /**
         *  Checks if a player is logged in (exists in the activeClientToId map)
         */
        bool
        isLoggedIn(const Connection &client);

        /**
         *  Checks if a client is in the process of registering a new Player
         */
        bool
        isRegistering(const Connection &client);

        /**
         *  Processes and responds to the input of a registering user based on the step they are in
         */
        std::string
        processRegistration(const Connection &client, const std::string &input = "");

        /**
         *  Removes client from the registration process. Used for disconnects
         */
        void
        exitRegistration(const Connection &client);

        /**
         *  Checks if a client is in the process of logging in
         */
        bool
        isLoggingIn(const Connection &client);

        /**
         *  Processes and responds to the input of user logging in based on the step they are in. Appends bootedClients
         *  if logging into a Player that is already being accessed by another client.
         */
        std::string
        processLogin(const Connection &client, const std::string &input = "");

        /**
         *  Removes client from the login process. Used for disconnects
         */
        void
        exitLogin(const Connection &client);

        /**
         *  Logs out the client and informs them.
         */
        std::string
        logoutPlayer(const Connection &client);

        /**
         *  Returns the username of a Player given a client ID. Used for displaying names in chat.
         */
        std::string
        getUsernameByClientId(const Connection &client);

        /**
         *  Appends Responses based on clients who have been logged out due to a login by another client into the
         *  same Player. Is to be called by the Game class' handleOutgoing() method.
         */
        void
        notifyBootedClients(std::deque<Response> &responses);

        static std::vector<Player>
        parseJsonUsers(json);
    };
}

#endif //PLAYERHANDLER_H