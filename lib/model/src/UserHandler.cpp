//
// Created by Stephen Wanhella on 2019-01-22.
//

#include <UserHandler.h>

#include "UserHandler.h"

model::UserHandler::UserHandler() {
  this->users = std::make_unique<UserCollection>();
}
