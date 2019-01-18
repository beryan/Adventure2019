//
// Created by arehal on 1/17/19.
//

#ifndef WEBSOCKETNETWORKING_WORLD_H
#define WEBSOCKETNETWORKING_WORLD_H

#include <map>
#include "User.h"

namespace model{
 class World{
 public:
     World();

 private:
     std::map<int, User> usersMap;
     //std::map<int, Area> areasMap;

 };
}

#endif //WEBSOCKETNETWORKING_WORLD_H


