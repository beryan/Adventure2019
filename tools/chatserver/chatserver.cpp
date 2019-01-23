/////////////////////////////////////////////////////////////////////////////
//                         Single Threaded Networking
//
// This file is distributed under the MIT License. See the LICENSE file
// for details.
/////////////////////////////////////////////////////////////////////////////


#include "Server.h"
#include "MessageResult.h"
#include "Door.h"
#include "Room.h"
#include "Player.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <map>
#include <vector>
#include <WorldHandler.h>

using model::WorldHandler;
using networking::Server;
using networking::Connection;
using networking::Message;
using model::Player;

static const char* const COMMAND_SHUTDOWN = "shutdown";
static const char* const COMMAND_QUIT = "quit";
static const char* const COMMAND_SAY = "say";
static const char* const COMMAND_HELP = "help";
static const char* const COMMAND_LOGOUT = "logout";
static const char* const COMMAND_REGISTER = "register";
static const char* const COMMAND_INFO = "info";
static const char* const COMMAND_START = "start";

std::vector<Connection> clients;
std::map<Connection, Player> logins;

std::optional<Player>
getPlayer(const Message& message) {
  std::optional<Player> player;
  auto iterator = logins.find(message.connection);
  if (iterator != logins.end()) {
    player = iterator->second;
  }
  return player;
}

std::string
lowercase(std::string string) {
  std::transform(string.begin(), string.end(), string.begin(), ::tolower);
  return string;
}


void
onConnect(Connection c) {
  std::cout << "New connection found: " << c.id << "\n";
  clients.push_back(c);
}


void
onDisconnect(Connection c) {
  std::cout << "Connection lost: " << c.id << "\n";
  auto eraseBegin = std::remove(std::begin(clients), std::end(clients), c);
  clients.erase(eraseBegin, std::end(clients));
}


std::deque<MessageResult>
processMessages(Server& server,
                const std::deque<Message>& incoming,
                bool& quit) {
  std::deque<MessageResult> results;

  for (auto& message : incoming) {
    auto result = MessageResult();
    auto connectionID = message.connection.id;
    auto player = getPlayer(message);

    result.setClientId(connectionID);

    std::ostringstream tempMessage;
    std::string action = lowercase(message.text.substr(0, message.text.find(' ')));
    std::string param;

    if (message.text.find(' ') != std::string::npos) {
      param = message.text.substr(message.text.find(' ') + 1);
    }

    if (action == COMMAND_QUIT) {
      server.disconnect(message.connection);
    } else if (action == COMMAND_SHUTDOWN) {
      std::cout << "Shutting down.\n";
      quit = true;
    } else if (action == COMMAND_SAY) {
      result.setPublic();
      tempMessage << connectionID << "> " << param << "\n";
    } else if (action == COMMAND_LOGOUT) {
      tempMessage << "Logout not yet implemented\n";
    } else if (action == COMMAND_REGISTER) {
      tempMessage << "Registering not yet implemented\n";
    } else if (action == COMMAND_START) {
      WorldHandler wh;
    } else if (action == COMMAND_HELP) {
      tempMessage << "********\n";
      tempMessage << "* HELP *\n";
      tempMessage << "********\n";
      tempMessage << "\n";
      tempMessage << "ACTIONS:\n";
      tempMessage << "  - " << COMMAND_HELP << " (shows this help interface)\n";
      tempMessage << "  - " << COMMAND_SAY << " [message] (sends [message] to other players in the game)\n";
      tempMessage << "  - " << COMMAND_REGISTER
                  << " [email password](registers user [email] with password [password])\n";
      tempMessage << "  - " << COMMAND_LOGOUT << " (logs you out of the server)\n";
      tempMessage << "  - " << COMMAND_QUIT << " (disconnects you from the game server)\n";
      tempMessage << "  - " << COMMAND_SHUTDOWN << " (shuts down the game server)\n";
      tempMessage << "  - " << COMMAND_INFO << " (displays current location information)\n";
    } else if (action == COMMAND_INFO) {
      model::Room stubRoom = model::Room();
      stubRoom.createStub();
      tempMessage << stubRoom;
    } else {
      tempMessage << "The word \"" << action << "\" is not a valid action. Enter \"help\" for a list of commands.\n";
    }

    result.setMessage(tempMessage.str());
    results.push_back(result);
  }

  return results;
}


std::deque<Message>
buildOutgoing(std::deque<MessageResult>& log) {
  std::deque<Message> outgoing;
  std::map<unsigned long int, std::ostringstream> clientMessages;

  for (auto entry : log) {
    if (entry.isLocal()) {
      clientMessages[entry.getClientId()] << entry.getMessage();

    } else {
      for (auto client : clients) {
        clientMessages[client.id] << entry.getMessage();
      }
    }
  }

  for (auto const&[clientId, message] : clientMessages) {
    outgoing.push_back({clientId, message.str()});
  }

  return outgoing;
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
              << "  e.g. " << argv[0] << " 4002 ./webchat.html\n";
    return 1;
  }

  bool done = false;
  unsigned short port = std::stoi(argv[1]);
  Server server{port, getHTTPMessage(argv[2]), onConnect, onDisconnect};

  while (!done) {
    try {
      server.update();
    } catch (std::exception& e) {
      std::cerr << "Exception from Server update:\n"
                << " " << e.what() << "\n\n";
      done = true;
    }

    auto incoming = server.receive();
    auto log = processMessages(server, incoming, done);
    auto outgoing = buildOutgoing(log);
    server.send(outgoing);
    sleep(1);
  }

  return 0;
}
