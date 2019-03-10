
#include <iostream>
#include "ClientMessageBuffer.h"

namespace networking {
    void
    ClientMessageBuffer::addMessageToQueue(const Message &message) {
        if (this->clientMessageQueue.count(message.connection)) {

            if (this->clientMessageQueue.at(message.connection).size() < MAX_BUFFER_MESSAGES) {
                this->clientMessageQueue.at(message.connection).emplace(message);

            } else {
                std::cout << message.connection.id << "'s message was discarded due to a having a full message buffer\n";
            }

        } else {
            this->clientMessageQueue[message.connection].emplace(message);
        }
    }

    void
    ClientMessageBuffer::addMessagesToQueues(const std::deque<Message> &messages) {
        for (auto const &message : messages) {
            this->addMessageToQueue(message);
        }
    }

    void
    ClientMessageBuffer::discardDisconnectedClientQueues(const std::vector<Connection> &disconnectedClients) {
        for (auto const &disconnectedClient : disconnectedClients) {
            this->clientMessageQueue.erase(disconnectedClient);
        }
    }

    std::deque<Message>
    ClientMessageBuffer::getNextMessages() {
        std::deque<Message> result;

        for (auto const &[client, queue]: this->clientMessageQueue) {
            if (!queue.empty()) {
                result.emplace_back(queue.front());
                this->clientMessageQueue.at(client).pop();
            }
        }

        return result;
    }
}
