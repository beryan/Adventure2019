//
// Created by louis on 19/01/19.
//

#ifndef GAME_H
#define GAME_H

#include "Server.h"
#include "Player.h"
#include "Response.h"
#include "PlayerHandler.h"

#include <functional>
#include <deque>
#include <map>

using networking::Connection;
using networking::Message;
using model::PlayerHandler;

namespace model {
    /**
     *  @class Game
     *
     *  @brief A class for containing the game's logic
     *
     *  This class manages the components necessary to gather, process, and output
     *  information that is to be exchanged between the server and clients.
     */
    class Game {
    private:
        const std::vector<Connection>* clients;
        std::vector<uintptr_t >* newClientIds;
        std::vector<uintptr_t >* disconnectedClientIds;
        std::function<void(Connection action)> disconnect;
        std::function<void()> shutdown;

        std::unique_ptr<PlayerHandler> playerHandler;

        static const char* const COMMAND_SHUTDOWN;
        static const char* const COMMAND_QUIT;
        static const char* const COMMAND_SAY;
        static const char* const COMMAND_HELP;
        static const char* const COMMAND_REGISTER;
        static const char* const COMMAND_LOGIN;
        static const char* const COMMAND_LOGOUT;
        static const char* const COMMAND_INFO;

        /**
         *  Calls handler class methods that manage newly connected clients. Empties new client IDs from the associated
         *  vector on completion.
         */
        void
        handleConnects(std::deque<Response> &responses);

        /**
         *  Calls handler class methods that manage disconnected users here. Empties disconnected client IDs from the
         *  associated vector on completion.
         */
        void
        handleDisconnects(std::deque<Response> &responses);

        /**
         *  Processes client input, calling class methods based on client input and formulating appropriate responses in
         *  the form of Response objects.
         */
        void
        handleIncoming(const std::deque<Message> &incoming, std::deque<Response> &responses);

        Response
        executeMenuAction(const uintptr_t &clientId, const std::string &command, const std::string &param);

        Response
        executeInGameAction(const uintptr_t &clientId, const std::string &command, const std::string &param);

        /**
         *  Calls handler class methods that return responses and are not dependent on user input.
         */
        void
        handleOutgoing(std::deque<Response> &responses);

        /**
         *  Converts a Response deque into a Message deque.
         */
        std::deque<Message>
        formMessages(std::deque<Response> &responses);

    public:
        /**
         *  Constructs a Game instance with references to connected clients, new client IDs, and disconnected client IDs.
         *  The game class' handleConnects() and handleDisconnects() methods are responsible for removing IDs from the
         *  new client ID and disconnected client ID vectors, respectively.
         */
        Game(std::vector<Connection> &clients,
             std::vector<uintptr_t > &newClientIds,
             std::vector<uintptr_t > &disconnectedClientIds,
             std::function<void(Connection)> &disconnect,
             std::function<void()> &shutdown);

        /**
         *  Runs a game cycle. Performs handleConnects(), handleDisconnects(), handleIncoming(), handleOutgoing(),
         *  and finally formMessages(). This Returns a deque of Message objects to be sent by the game server.
         */
        std::deque<Message>
        processCycle(std::deque<Message> &incoming);
    };
}

#endif //GAME_H
