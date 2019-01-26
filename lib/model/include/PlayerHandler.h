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

using json = nlohmann::json;

namespace model {
    typedef int PlayerId;

    class PlayerHandler {
    public:
        PlayerHandler();

        /**
         *  Checks if a player is logged in (exists in the activeClientToId map)
         */
        bool
        isLoggedIn(const uintptr_t &clientId);

        /**
         *  Returns the result of a registration attempt
         */
        std::string
        registerPlayer(const uintptr_t &clientId, const std::string &param);

        /**
         *  Returns the result of a login attempt. Appends bootedClients if logging into a Player that is already
         *  being accessed by another client.
         */
        std::string
        loginPlayer(const uintptr_t &clientId, const std::string &param);

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
         *  Appends Responses based on clients who have been logged out do to a login by another client into the
         *  same Player. Is to be called by the Game class' handleOutgoing() method.
         */
        void
        notifyBootedClients(std::deque<Response> &responses);

        static std::vector<Player>
        parseJsonUsers(json);

    private:
        PlayerId nextId;
        std::map<PlayerId, Player> allPlayers;
        std::map<std::string, Player*> usernameToPlayer;
        std::map<PlayerId, uintptr_t> activeIdToClient;
        std::map<uintptr_t, PlayerId> activeClientToId;

        std::vector<uintptr_t> bootedClients;
  };
}

#endif //PLAYERHANDLER_H
