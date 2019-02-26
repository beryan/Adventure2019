//
// Created by louis on 19/01/19.
//

#ifndef GAME_H
#define GAME_H

#include "Server.h"
#include "Player.h"
#include "PlayerHandler.h"
#include "WorldHandler.h"
#include "Command.h"
#include "CommandHandler.h"

#include <functional>
#include <deque>
#include <map>

using networking::Connection;
using networking::Message;
using model::PlayerHandler;
using model::WorldHandler;
using model::Command;

namespace model {
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
        model::CommandHandler commandHandler;

        std::unique_ptr<PlayerHandler> playerHandler;
        std::unique_ptr<WorldHandler> worldHandler;

        /**
         *  Calls handler class methods that manage newly connected clients. Empties new client IDs from the associated
         *  vector on completion.
         */
        void
        handleConnects(std::deque<Message> &responses);

        /**
         *  Calls handler class methods that manage disconnected users here. Empties disconnected client IDs from the
         *  associated vector on completion.
         */
        void
        handleDisconnects(std::deque<Message> &responses);

        /**
         *  Processes client input, calling class methods based on client input and formulating appropriate responses in
         *  the form of Response objects.
         */
        void
        handleIncoming(const std::deque<Message> &incoming, std::deque<Message> &messages);

        /**
         *  Creates a Response to commands when the client is not logged in
         */
        Message
        executeMenuAction(const Connection &client, const Command &command, const std::string &param);

        /**
         *  Creates a Response to commands when the client is logged in
         */
        std::vector<Message>
        executeInGameAction(const Connection &client, const Command &command, const std::string &param);

        /**
         *  Calls handler class methods that return responses and are not dependent on user input.
         */
        void
        handleOutgoing(std::deque<Message> &messages);


        /**
         *  Combines messages such that all clients will receive one message at most per game cycle.
         */
        std::deque<Message>
        formMessages(std::deque<Message> &messages);

        /**
         *  Returns the words associated with a command in the form of a comma-separated string.
         *  Used to display commands in the help display.
         */
        std::string
        getCommandWords(Command command);

        /**
         *  Update game state to include connections that are in game.
         */
        void
        addClientToGame(Connection client);

        /**
         *  Update game state to not include connections that are no longer in game.
         */
        void
        removeClientFromGame(Connection client);

        /**
         *  Checks if parameters are incorrect format for in game command.
         */
        bool
        isInvalidFormat(const Command &command, const std::string &parameters);

    public:
        /**
         *  Constructs a Game instance with references to connected clients, new client IDs, and disconnected client IDs.
         *  The game class' handleConnects() and handleDisconnects() methods are responsible for removing IDs from the
         *  new client ID and disconnected client ID vectors, respectively.
         */
        Game(std::vector<Connection> &clients,
             std::vector<Connection> &newClients,
             std::vector<Connection> &disconnectedClients,
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
