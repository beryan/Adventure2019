//
// Created by Stephen Wanhella on 2019-01-19.
//

#include "User.h"
using model::User;

bool User::operator==(const User& user) {
  return id == user.id;
}