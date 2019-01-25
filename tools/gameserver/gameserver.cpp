/////////////////////////////////////////////////////////////////////////////
//                         Single Threaded Networking
//
// This file is distributed under the MIT License. See the LICENSE file
// for details.
/////////////////////////////////////////////////////////////////////////////


#include "Server.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <vector>
#include <Game.h>


using networking::Server;
using networking::Connection;
using networking::Message;
using model::Game;

std::vector<Connection> clients;
std::vector<uintptr_t> newClientIds;
std::vector<uintptr_t> disconnectedClientIds;

void
onConnect(Connection c) {
  std::cout << "New connection found: " << c.id << "\n";
  clients.push_back(c);
  newClientIds.push_back(c.id);
}


void
onDisconnect(Connection c) {
  std::cout << "Connection lost: " << c.id << "\n";
  auto eraseBegin = std::remove(std::begin(clients), std::end(clients), c);
  clients.erase(eraseBegin, std::end(clients));
  disconnectedClientIds.push_back(c.id);
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

  Game game(clients, newClientIds, disconnectedClientIds, disconnect, shutdown);

  while (!done) {
    try {
      server.update();

    } catch (std::exception& e) {
      std::cerr << "Exception from Server update:\n"
                << " " << e.what() << "\n\n";
      done = true;
    }

    std::deque<Message> incoming = server.receive();
    std::deque<Message> result = game.processCycle(incoming);
    server.send(result);

    sleep(1);
  }

  return 0;
}
