//
// Created by steph on 3/14/2019.
//

#include <ConnectionHandler.h>

#include "ConnectionHandler.h"

ConnectionHandler::ConnectionHandler(std::vector<Connection> &clients,
                                     std::vector<Connection> &newClients,
                                     std::vector<Connection> &disconnectedClients,
                                     std::function<void(Connection)> disconnectFn,
                                     std::function<void()> shutdownFn)
        : clients(clients),
          newClients(newClients),
          disconnectedClients(disconnectedClients),
          disconnectFn(std::move(disconnectFn)),
          shutdownFn(std::move(shutdownFn)) {

}

std::vector<Connection> &ConnectionHandler::getClients() {
    return this->clients;
}

std::vector<Connection> &ConnectionHandler::getNewClients() {
    return this->newClients;
}

std::vector<Connection> &ConnectionHandler::getDisconnectedClients() {
    return this->disconnectedClients;
}

void ConnectionHandler::shutdown() {
    this->shutdownFn();
}

void ConnectionHandler::disconnect(Connection client) {
    this->disconnectFn(client);
}
