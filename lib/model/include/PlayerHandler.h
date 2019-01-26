//
// Created by Stephen Wanhella on 2019-01-22.
//

#ifndef PLAYERHANDLER_H
#define PLAYERHANDLER_H

#include "Player.h"
#include "Response.h"
#include "json.hpp"

#include <map>
#include <deque>

using json = nlohmann::json;

namespace model {
  typedef int PlayerId;

  class PlayerHandler {
  public:
      PlayerHandler();

      bool isLoggedIn(const uintptr_t &clientId);

      std::string registerPlayer(const uintptr_t &clientId, const std::string &param);

      std::string loginPlayer(const uintptr_t &clientId, const std::string &param);

      std::string logoutPlayer(const uintptr_t &clientId);

      std::string getUsernameByClientId(const uintptr_t &clientId);

      static std::vector<Player> parseJsonUsers(json);

      void notifyBootedClients(std::deque<Response> &responses);

  private:
    PlayerId nextId;
    std::map<PlayerId, Player> allPlayers;
    std::map<std::string, Player*> usernameToPlayer;
    std::map<PlayerId, uintptr_t> activeIdToClient;
    std::map<uintptr_t, PlayerId> activeClientToId;

    std::vector<uintptr_t> bootedClients;
  };
}

#endif //PLAYERHANDLER_H
