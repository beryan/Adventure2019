//
// Created by louis on 19/01/19.
//

#ifndef WEBSOCKETNETWORKING_GAME_H
#define WEBSOCKETNETWORKING_GAME_H

#include "GameResponse.h"
#include "Server.h"
#include "Greeter.h"

#include <functional>
#include <deque>

using model::Greeter;
using networking::Connection;
using networking::Message;

namespace model {
    /**
     * @class Game
     *
     * @brief A class for containing the game's logic
     *
     * This class manages the componentsts necessary to gather, process, and output
     * information that is to be exchanged between the server and clients.
     */
    class Game {
    private:
        const std::vector<Connection>* clients;
        std::vector<long unsigned int>* newClientIds;
        std::vector<long unsigned int>* disconnectedClientIds;
        std::function<void(Connection action)> disconnect;
        std::function<void()> shutdown;
        std::unique_ptr<Greeter> greeter;

        /**
         * Processes client input, calling class methods based on client input and formulating appropriate responses in
         * the form of ActionResult objects stored in a deque. This returns a deque of ActionResult objects.
         */
        std::deque<GameResponse>
        handleIncoming(const std::deque<Message> &incoming);

        /**
         * Calls handler class methods that are not dependent on user input for sending responses.
         */
        void
        handleEvents(std::deque<GameResponse> &results);

        /**
         * Converts ActionResult objects into Message objects to be sent to the appropriate client(s). This returns
         * a deque of Message objects to be sent by the game server.
         */
        std::deque<Message>
        handleOutgoing(std::deque<GameResponse> &outgoing);

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
         * Calls handler class methods that manage newly connected clients. Empties new client IDs from the associated
         * vector on completion.
         */
        void
        handleConnects();

        /**
         * Calls handler class methods that manage disconnected users here. Empties disconnected client IDs from the
         * associated vector on completion.
         */
        void
        handleDisconnects();

        /**
         * Runs a game cycle. Performs handleConnects()handleIncoming(), handleEvents(), and handleOutgoing() in respective order.
         * This Returns a deque of Message objects to be sent by the game server.
         */
        std::deque<Message>
        processCycle(std::deque<Message> &incoming);
    };
}

#endif //WEBSOCKETNETWORKING_GAME_H
