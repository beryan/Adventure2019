//
// Created by arehal on 1/18/19.
//

#include <fstream>
#include <iostream>
#include <string>

#include "WorldHandler.h"


using model::WorldHandler;

using json = nlohmann::json;

namespace model {

    WorldHandler::WorldHandler(){
        std::cout << "WorldHandler Created" << std::endl;
        parseJSON();
    }

    void WorldHandler::addUser(Player player){
        std::cout << "Adding " << player.getId() << " " << player.getUsername() << " to users" << std::endl;
        this->world.insertUser(player);
    }

    void WorldHandler::removeUser(Player player){
        this->world.getUserMap().erase(player.getId());
    }

//    void WorldHandler::addArea(Area area){
//        this->world.getAreaMap().insert(std::make_pair(area.getId(), area));
//    }
//
//    void WorldHandler::removeArea(Area area){
//        this->world.getAreaMap().erase(area.getId());
//    }

    void WorldHandler::parseJSON() {
        std::ifstream ifs("users.json");
//        std::ifstream input("users.json");
        json t = json::parse(ifs);

        json users;

        for (json::iterator it = t.begin(); it != t.end(); ++it) {
            if(it.key() == "USERS"){
                users = it.value();
            }
        }

        createUsersFromJSON(users);
        world.printUsers();
    }

    void WorldHandler::createUsersFromJSON(json users){
        for (json::iterator it = users.begin(); it != users.end(); ++it) {
//            std::cout << it.value().at("password") << std::endl;
            model::Player p (it.value().at("id"), it.value().at("username"), 1232442);
            addUser(p);
        }
    }


}