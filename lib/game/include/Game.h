//
// Created by louis on 19/01/19.
//

#ifndef GAME_H
#define GAME_H

#include "Server.h"
#include "Player.h"
#include "AccountHandler.h"
#include "AvatarHandler.h"
#include "WorldHandler.h"
#include "MagicHandler.h"
#include "Command.h"
#include "AliasManager.h"
#include "PlayerHandler.h"
#include "CommandParser.h"
#include "CombatHandler.h"
#include "CommandExecutor.h"
#include "DataManager.h"

#include <functional>
#include <deque>
#include <map>
#include <ConnectionHandler.h>

using networking::Connection;
using networking::Message;
using handler::AccountHandler;
using handler::WorldHandler;
using handler::PlayerHandler;
using handler::MagicHandler;
using handler::CombatHandler;
using handler::AvatarHandler;
using game::Command;
using game::AliasManager;
using game::CommandExecutor;

namespace game {
    /**
     *  @class Game
     *
     *  @brief A class for containing the game's logic
     *
     *  This class manages the components necessary to gather, process, and output
     *  information that is to be exchanged between the server and clients. This
     *  class expects the server to keep track of newly connected and disconnected
     *  clients by adding the Connection object to the respective vector. The Game
     *  instance will remove the Connections from the vectors after reading them.
     */
    class Game {
    private:
        ConnectionHandler &connectionHandler;
        AccountHandler accountHandler;
        AvatarHandler avatarHandler;
        MagicHandler magicHandler;
        WorldHandler worldHandler;
        AliasManager aliasManager;
        CommandParser commandParser;
        CombatHandler combatHandler;
        CommandExecutor commandExecutor;

        bool running;

        /**
         *  Calls handler class methods that manage newly connected clients. Empties new client Connections from the
         *  associated vector on completion.
         */
        void
        handleConnects(std::deque<Message> &responses);

        /**
         *  Calls handler class methods that manage disconnected users here. Empties disconnected client Connections
         *  from the associated vector on completion.
         */
        void
        handleDisconnects(std::deque<Message> &responses);

        /**
         *  Processes client input Messages, calling class methods based on their input and formulating appropriate
         *  Messages to return to clients.
         */
        void
        handleIncoming(const std::deque<Message> &incoming, std::deque<Message> &messages);

        /**
         *  Processes commands when the client is not logged in. Returns a single Message.
         */
        Message
        executeMenuAction(const Connection &client, const Command &command, const std::string &param);

        /**
         *  Processes commands when the client is logged in. Returns a vector of Messages.
         */
        std::vector<Message>
        executeInGameAction(const Connection &client, const Command &command, const std::string &param);

        /**
         *  Calls handler class methods that appends event-based Messages to the passed in vector.
         */
        void
        handleOutgoing(std::deque<Message> &messages);

        /**
         *  Combines Messages such that all clients will receive one Message per game cycle.
         */
        std::deque<Message>
        formMessages(std::deque<Message> &messages);

        /**
         *  Update game state to include Connections that are in-game.
         */
        void
        addClientToGame(Connection client);

        /**
         *  Update game state to not include Connections that are no longer in-game.
         */
        std::string
        removeClientFromGame(Connection client);

        /**
         *  Checks if parameters are in incorrect format for in-game commands.
         */
        bool
        isInvalidFormat(const Command &command, const std::string &parameters);

    public:
        /**
         *  Constructs a Game instance with references to connected clients, new client IDs, and disconnected client IDs.
         *  The game class' handleConnects() and handleDisconnects() methods are responsible for removing Connections
         *  from the newClients and disconnectedClients vectors, respectively.
         */
        explicit Game(ConnectionHandler &connectionManager);

        /**
         *  Runs a game cycle. Performs handleConnects(), handleDisconnects(), handleIncoming(), handleOutgoing(),
         *  and finally formMessages(). This returns a deque of Messages to be sent to clients by the server.
         */
        std::deque<Message>
        processCycle(std::deque<Message> &incoming);

        /**
         * Checks if the game is still running
         * @return true if the game is still running, false otherwise
         */
        bool isRunning() const;

        /**
         * Shuts down the game
         */
        void shutdown();

    };
}

#endif //GAME_H
