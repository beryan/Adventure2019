//
// Created by arehal on 1/17/19.
//

#ifndef WEBSOCKETNETWORKING_WORLD_H
#define WEBSOCKETNETWORKING_WORLD_H

#include <map>
#include "Player.h"

namespace model{
 class World{
 public:
     World();

     std::map<int, Player> getUserMap();
//     std::map<int, Character> getAreaMap();

    void insertUser(Player player);

     void printUsers();

 private:
     std::map<int, Player> usersMap;
     //std::map<int, Area> areasMap;

 };
}

#endif //WEBSOCKETNETWORKING_WORLD_H


