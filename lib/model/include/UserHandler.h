//
// Created by Stephen Wanhella on 2019-01-22.
//

#ifndef WEBSOCKETNETWORKING_USERHANDLER_H
#define WEBSOCKETNETWORKING_USERHANDLER_H

#include "UserCollection.h"

namespace model {
  class UserHandler {
  public:
    UserHandler();

  private:
    std::unique_ptr<UserCollection> users;
  };
}

#endif //WEBSOCKETNETWORKING_USERHANDLER_H
