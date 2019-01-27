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
    typedef int PlayerId;

    class PlayerHandler {
    private:
        static const unsigned short MIN_PASSWORD_LENGTH;
        static const unsigned short MAX_USERNAME_AND_PASSWORD_LENGTH;

        PlayerId nextId;
        std::map<PlayerId, Player> allPlayers;
        std::map<std::string, Player*> usernameToPlayer;
        std::map<PlayerId, uintptr_t> activeIdToClient;
        std::map<uintptr_t, PlayerId> activeClientToId;

        std::map<uintptr_t, std::string> regUsernameInput;
        std::map<uintptr_t, std::string> regPasswordInput;
        std::unordered_set<uintptr_t> regUsernamePrompt;
        std::unordered_set<uintptr_t> regPasswordFirstPrompt;
        std::unordered_set<uintptr_t> regPasswordSecondPrompt;

        std::map<uintptr_t, std::string> loginUsernameInput;
        std::unordered_set<uintptr_t> loginUsernamePrompt;
        std::unordered_set<uintptr_t> loginPasswordPrompt;

        std::vector<uintptr_t> bootedClients;

    public:
        PlayerHandler();

        /**
         *  Checks if a player is logged in (exists in the activeClientToId map)
         */
        bool
        isLoggedIn(const uintptr_t &clientId);


        /**
         *  Add clients to the registration process
         */
        std::string
        startRegistration(const uintptr_t &clientId);

        /**
         *  Checks if a client is in the process of registering a new Player
         */
        bool
        isRegistering(const uintptr_t &clientId);

        /**
         *  Processes and responds to the input of a registering user based on the step they are in
         */
        std::string
        processRegistration(const uintptr_t &clientId, const std::string &param);

        /**
         *  Checks if a client is in the process of logging in
         */
        bool
        isLoggingIn(const uintptr_t &clientId);

        /**
         *  Add clients to the login process
         */
        std::string
        startLogin(const uintptr_t &clientId);

        /**
         *  Processes and responds to the input of user logging in based on the step they are in. Appends bootedClients
         *  if logging into a Player that is already being accessed by another client.
         */
        std::string
        processLogin(const uintptr_t &clientId, const std::string &param);

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
