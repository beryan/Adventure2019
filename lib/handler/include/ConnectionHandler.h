//
// Created by steph on 3/14/2019.
//

#ifndef WEBSOCKETNETWORKING_CONNECTIONHANDLER_H
#define WEBSOCKETNETWORKING_CONNECTIONHANDLER_H

#include "Server.h"

using networking::Connection;

class ConnectionHandler {
public:
    ConnectionHandler(std::vector<Connection> &clients,
                      std::vector<Connection> &newClients,
                      std::vector<Connection> &disconnectedClients,
                      std::function<void(const Connection &)> disconnect);

    std::vector<Connection> &getClients();

    std::vector<Connection> &getNewClients();

    std::vector<Connection> &getDisconnectedClients();

    void disconnectClient(const Connection &client);

private:
    std::vector<Connection> &clients;
    std::vector<Connection> &newClients;
    std::vector<Connection> &disconnectedClients;
    std::function<void(const Connection &client)> disconnectFn;
};


#endif //WEBSOCKETNETWORKING_CONNECTIONHANDLER_H
