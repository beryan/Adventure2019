/////////////////////////////////////////////////////////////////////////////
//                         Single Threaded Networking
//
// This file is distributed under the MIT License. See the LICENSE file
// for details.
/////////////////////////////////////////////////////////////////////////////


#include "Server.h"
#include <ClientMessageBuffer.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <vector>
#include <Game.h>


using networking::Server;
using networking::Connection;
using networking::Message;
using networking::ClientMessageBuffer;
using model::Game;

std::vector<Connection> clients;
std::vector<Connection> newClients;
std::vector<Connection> disconnectedClients;

void
onConnect(Connection c) {
  std::cout << "New connection found: " << c.id << "\n";
  clients.push_back(c);
  newClients.push_back(c);
}


void
onDisconnect(Connection c) {
  std::cout << "Connection lost: " << c.id << "\n";
  auto eraseBegin = std::remove(std::begin(clients), std::end(clients), c);
  clients.erase(eraseBegin, std::end(clients));
  disconnectedClients.push_back(c);
}


std::string
getHTTPMessage(const char* htmlLocation) {
  if (access(htmlLocation, R_OK) != -1) {
    std::ifstream infile{htmlLocation};
    return std::string{std::istreambuf_iterator<char>(infile),
                       std::istreambuf_iterator<char>()};

  } else {
    std::cerr << "Unable to open HTML index file:\n"
              << htmlLocation << "\n";
    std::exit(-1);
  }
}


int
main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cerr << "Usage:\n  " << argv[0] << " <port> <html response>\n"
              << "  e.g. " << argv[0] << " 4002 ./webgame.html\n";
    return 1;
  }

  bool done = false;
  unsigned short port = (unsigned short) std::stoi(argv[1]);
  Server server{port, getHTTPMessage(argv[2]), onConnect, onDisconnect};

  std::function<void(Connection)> disconnect = [&server](Connection connection) {
      server.disconnect(connection);
  };

  std::function<void()> shutdown = [&done]() {
      done = true;
  };

  ClientMessageBuffer clientMessageBuffer;
  Game game{clients, newClients, disconnectedClients, disconnect, shutdown};

  while (!done) {
    try {
      server.update();

    } catch (std::exception& e) {
      std::cerr << "Exception from Server update:\n"
                << " " << e.what() << "\n\n";
      done = true;
    }

    std::deque<Message> incoming = server.receive();

    clientMessageBuffer.addMessagesToQueues(incoming);
    clientMessageBuffer.discardDisconnectedClientQueues(disconnectedClients);

    auto next = clientMessageBuffer.getNextMessages();
    server.send(game.processCycle(next));

    sleep(1);
  }

  return 0;
}
