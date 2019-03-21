//
// Created by louis on 19/01/19.
//

#ifndef GAME_H
#define GAME_H

#include "Server.h"
#include "Player.h"
#include "AccountHandler.h"
#include "WorldHandler.h"
#include "MagicHandler.h"
#include "Command.h"
#include "AliasManager.h"
#include "PlayerHandler.h"
#include "CommandParser.h"
#include "AvatarHandler.h"

#include <functional>
#include <deque>
#include <map>

using networking::Connection;
using networking::Message;
using handler::AccountHandler;
using handler::WorldHandler;
using handler::PlayerHandler;
using handler::MagicHandler;
using handler::AvatarHandler;
using game::Command;
using game::AliasManager;

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
        std::vector<Connection>* clients;
        std::vector<Connection>* newClients;
        std::vector<Connection>* disconnectedClients;
        std::function<void(Connection action)> disconnect;
        std::function<void()> shutdown;

        AccountHandler accountHandler;
        AvatarHandler avatarHandler;
        MagicHandler magicHandler;
        PlayerHandler playerHandler;
        WorldHandler worldHandler;
        AliasManager aliasManager;
        CommandParser commandParser;

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
        void
        removeClientFromGame(Connection client);

        /**
         *  Checks if parameters are in incorrect format for in-game commands.
         */
        bool
        isInvalidFormat(const Command &command, const std::string &parameters);

        template<typename T>
        bool
        containsKeyword(const std::vector<T> &objects, const std::string &keyword);

        template<typename T>
        T
        getItemByKeyword(const std::vector<T> &objects, const std::string &keyword);

    public:
        /**
         *  Constructs a Game instance with references to connected clients, new client IDs, and disconnected client IDs.
         *  The game class' handleConnects() and handleDisconnects() methods are responsible for removing Connections
         *  from the newClients and disconnectedClients vectors, respectively.
         */
        Game(std::vector<Connection> &clients,
             std::vector<Connection> &newClients,
             std::vector<Connection> &disconnectedClients,
             std::function<void(Connection)> &disconnect,
             std::function<void()> &shutdown);

        /**
         *  Runs a game cycle. Performs handleConnects(), handleDisconnects(), handleIncoming(), handleOutgoing(),
         *  and finally formMessages(). This returns a deque of Messages to be sent to clients by the server.
         */
        std::deque<Message>
        processCycle(std::deque<Message> &incoming);
    };
}

#endif //GAME_H
