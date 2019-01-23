//
// Created by Stephen Wanhella on 2019-01-19.
//

#ifndef WEBSOCKETNETWORKING_USERCOLLECTION_H
#define WEBSOCKETNETWORKING_USERCOLLECTION_H

#include <array>
#include <memory>
#include "User.h"

using model::User;

namespace model {
  class UserCollection {
  public:
    UserCollection() = default;

    explicit UserCollection(std::vector<User>);

    void addUser(const User&);

    User getUser(unsigned int index);

    unsigned long getSize();

    void deleteUser(unsigned int index);

    long findUser(const User&);

  private:
    std::vector<User> users;
  };
}

#endif //WEBSOCKETNETWORKING_USERCOLLECTION_H
