//
// Created by louis on 19/01/19.
//

#ifndef WEBSOCKETNETWORKING_GAME_H
#define WEBSOCKETNETWORKING_GAME_H

#include "GameResponse.h"
#include "Server.h"
#include "Player.h"

#include <functional>
#include <deque>
#include <map>

using networking::Connection;
using networking::Message;

namespace model {
    /**
     * @class Game
     *
     * @brief A class for containing the game's logic
     *
     * This class manages the components necessary to gather, process, and output
     * information that is to be exchanged between the server and clients.
     */
    class Game {
    private:
        const std::vector<Connection>* clients;
        std::vector<long unsigned int>* newClientIds;
        std::vector<long unsigned int>* disconnectedClientIds;
        std::function<void(Connection action)> disconnect;
        std::function<void()> shutdown;

        static const char* const COMMAND_SHUTDOWN;
        static const char* const COMMAND_QUIT;
        static const char* const COMMAND_SAY;
        static const char* const COMMAND_HELP;
        static const char* const COMMAND_REGISTER;
        static const char* const COMMAND_LOGIN;
        static const char* const COMMAND_LOGOUT;
        static const char* const COMMAND_INFO;
        static const char* const COMMAND_START;

        /* Login/Register member variables */
        std::map<std::string, std::string> tempUserToPass;
        std::map<std::string, int> tempUserToId;
        std::map<int, Player> tempIdToPlayer;
        std::map<int, unsigned long int> activeIdToClient;
        /* End */

        std::map<unsigned long int, int> activePlayerList;

        /**
         * Calls handler class methods that manage newly connected clients. Empties new client IDs from the associated
         * vector on completion.
         */
        void
        handleConnects(std::deque<GameResponse> &results);

        /**
         * Calls handler class methods that manage disconnected users here. Empties disconnected client IDs from the
         * associated vector on completion.
         */
        void
        handleDisconnects(std::deque<GameResponse> &results);

        /**
         * Processes client input, calling class methods based on client input and formulating appropriate responses in
         * the form of GameResponse objects.
         */
        void
        handleIncoming(const std::deque<Message> &incoming, std::deque<GameResponse> &results);

        /**
         * Calls handler class methods that return responses and are not dependent on user input.
         */
        void
        handleOutgoing(std::deque<GameResponse> &results);

        /**
         * Converts a GameResponse deque into a Message deque.
         */
        std::deque<Message>
        formMessages(std::deque<GameResponse> &results);

        std::optional<Player>
        getPlayer(const unsigned long int& clientId);

    public:
        /**
         * Constructs a Game instance with references to connected clients, new client IDs, and disconnected client IDs.
         * The game class' handleConnects() and handleDisconnects() methods are responsible for removing IDs from the
         * new client ID and disconnected client ID vectors, respectively.
         */
        Game(std::vector<Connection> &clients,
             std::vector<unsigned long int> &newClientIds,
             std::vector<unsigned long int> &disconnectedClientIds,
             std::function<void(Connection)> &disconnect,
             std::function<void()> &shutdown);

        /**
         * Runs a game cycle. Performs handleConnects(), handleDisconnects(), handleIncoming(), handleOutgoing(),
         * and finally formMessages(). This Returns a deque of Message objects to be sent by the game server.
         */
        std::deque<Message>
        processCycle(std::deque<Message> &incoming);
    };
}

#endif //WEBSOCKETNETWORKING_GAME_H
