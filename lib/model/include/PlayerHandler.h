//
// Created by Stephen Wanhella on 2019-01-22.
//

#ifndef WEBSOCKETNETWORKING_USERHANDLER_H
#define WEBSOCKETNETWORKING_USERHANDLER_H

#include <map>
#include "Player.h"

namespace model {
  class PlayerHandler {
  public:
    PlayerHandler() = default;
    Player registerPlayer(std::string username, std::string password);
    Player login(std::string username, std::string password);
    bool isRegistered(std::string);
//    void parseJSONUsers();
  private:
    std::map<std::string, int> usernameToIdMap;
    std::map<int, Player> idToPlayerMap;
  };
}

#endif //WEBSOCKETNETWORKING_USERHANDLER_H
