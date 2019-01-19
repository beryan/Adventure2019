//
// Created by Stephen Wanhella on 2019-01-19.
//

#ifndef WEBSOCKETNETWORKING_USERCOLLECTION_H
#define WEBSOCKETNETWORKING_USERCOLLECTION_H

#include <array>
#include <memory>
#include "User.h"

namespace model {
    class UserCollection {
    public:
      static UserCollection* Instance();
      void addUser(const User&);
      User getUser(unsigned int index);
      unsigned long getSize();
      void deleteUser(unsigned int index);
      long findUser(const User&);

    private:
      std::vector<model::User> users;
      UserCollection() = default;
      UserCollection(UserCollection const&);
      UserCollection& operator=(UserCollection const&) {};
      static UserCollection* instance;
    };
}

#endif //WEBSOCKETNETWORKING_USERCOLLECTION_H
