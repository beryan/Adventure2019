//
// Created by arehal on 1/17/19.
//

#ifndef WEBSOCKETNETWORKING_WORLD_H
#define WEBSOCKETNETWORKING_WORLD_H

namespace model{
 class World{
 public:
     World();

     //
     void LoadAreas();

     void LoadUsers();

     // given a json file, create Areas
     void ParseAreas();

     // given a json file, create Users
     void ParseUsers();

 private:
     std::vector<Area> areas;
     std::vector<User> users;
 };
}

#endif //WEBSOCKETNETWORKING_WORLD_H


