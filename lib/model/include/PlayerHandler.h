//
// Created by Stephen Wanhella on 2019-01-22.
//

#ifndef PLAYERHANDLER_H
#define PLAYERHANDLER_H

#include "Player.h"
#include "Response.h"
#include "json.hpp"

#include <map>
#include <deque>
#include <unordered_set>

using json = nlohmann::json;

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
        std::map<model::ID, uintptr_t> activeIdToClient;
        std::map<uintptr_t, model::ID> activeClientToId;

        std::map<uintptr_t, std::string> regUsernameInput;
        std::map<uintptr_t, std::string> regPasswordInput;
        std::map<uintptr_t, RegisterStage> clientRegisterStage;

        std::map<uintptr_t, std::string> loginUsernameInput;
        std::map<uintptr_t, LoginStage> clientLoginStage;

        std::vector<uintptr_t> bootedClients;

    public:
        PlayerHandler();

        /**
         *  Checks if a player is logged in (exists in the activeClientToId map)
         */
        bool
        isLoggedIn(const uintptr_t &clientId);

        /**
         *  Checks if a client is in the process of registering a new Player
         */
        bool
        isRegistering(const uintptr_t &clientId);

        /**
         *  Processes and responds to the input of a registering user based on the step they are in
         */
        std::string
        processRegistration(const uintptr_t &clientId, const std::string &input = "");

        /**
         *  Removes client from the registration process. Used for disconnects
         */
        void
        exitRegistration(const uintptr_t &clientId);

        /**
         *  Checks if a client is in the process of logging in
         */
        bool
        isLoggingIn(const uintptr_t &clientId);

        /**
         *  Processes and responds to the input of user logging in based on the step they are in. Appends bootedClients
         *  if logging into a Player that is already being accessed by another client.
         */
        std::string
        processLogin(const uintptr_t &clientId, const std::string &input = "");

        /**
         *  Removes client from the login process. Used for disconnects
         */
        void
        exitLogin(const uintptr_t &clientId);

        /**
         *  Logs out the client and informs them.
         */
        std::string
        logoutPlayer(const uintptr_t &clientId);

        /**
         *  Returns the username of a Player given a client ID. Used for displaying names in chat.
         */
        std::string
        getUsernameByClientId(const uintptr_t &clientId);

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