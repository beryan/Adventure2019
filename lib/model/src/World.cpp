//
// Created by arehal on 1/17/19.
//

#include "World.h"
#include "User.h"

using model::World;

namespace model {

    World::World(){}

    std::map<int, User> World::getUserMap(){
        return this->usersMap;
    }

//    std::map<int, Area> World::getAreaMap(){
//        return this->areasMap;
//    }
};