#ifndef CLIENTMESSAGEBUFFER_H
#define CLIENTMESSAGEBUFFER_H

#include <map>
#include <queue>
#include "Server.h"

using networking::Connection;
using networking::Message;

namespace networking {

unsigned int constexpr MAX_BUFFER_MESSAGES = 10;

/**
 *  @class ClientMessageBuffer
 *
 *  @brief A class for queuing a client's incoming messages to the Game
 *
 *  This class manages the message queues of clients connected to the server.
 *  The main functions this class are adding messages to
 *  their associated queues, retrieving the next message in each queue, and
 *  deleting the queues of disconnected clients.
 */
class ClientMessageBuffer {
private:
    std::map<Connection, std::queue<Message>> clientMessageQueue;

    /**
     *  Adds a message to the sending client's queue.
     */
    void
    addMessageToQueue(const Message &message);

public:
    ClientMessageBuffer() = default;

    /**
     *  Places a each message in a deque to each sending client's queue.
     */
    void
    addMessagesToQueues(const std::deque<Message> &messages);

    /**
     *  Erases the queue of each client listed in the provided vector.
     */
    void
    discardDisconnectedClientQueues(const std::vector<Connection> &disconnectedClients);

    /**
     *  Returns a deque of messages consisting of the next message in each client's queue.
     */
    std::deque<Message>
    getNextMessages();
};

}

#endif //CLIENTMESSAGEBUFFER_H
