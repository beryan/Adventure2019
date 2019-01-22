//
// Created by arehal on 1/17/19.
//

#include <iostream>

#include "World.h"

using model::World;
using model::Player;

namespace model {

    World::World(){}

    std::map<int, Player> World::getUserMap(){
        return this->usersMap;
    }

    void World::insertUser(Player player){
        usersMap.insert(std::pair<int,Player>(player.getId(), player));
    }

//    std::map<int, Area> World::getAreaMap(){
//        return this->areasMap;
//    }

    void World::printUsers() {
        std::cout << "Users:" << std::endl;

        for (auto& [key, value]: usersMap) {
            std::cout << value.getId() << ". " <<  value.getUsername() << std::endl;
        }
    }
};