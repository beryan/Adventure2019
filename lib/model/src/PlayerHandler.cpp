//
// Created by Stephen Wanhella on 2019-01-22.
//

#include <PlayerHandler.h>

#include "PlayerHandler.h"

using json = nlohmann::json;
using player = model::Player;

namespace model {

  std::vector<Player> PlayerHandler::parseJsonUsers(json users) {
    std::vector<Player> players;

    for (json::iterator it = users.begin(); it != users.end(); ++it) {
      Player p (it.value().at("id"), it.value().at("username"));
      players.push_back(p);
    }

    return players;
  }
}
