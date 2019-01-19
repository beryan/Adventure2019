//
// Created by Stephen Wanhella on 2019-01-19.
//

#include <UserCollection.h>
#include <algorithm>

#include "UserCollection.h"
using model::UserCollection;

UserCollection* UserCollection::instance = nullptr;

UserCollection* UserCollection::Instance() {
  if (!instance) {
    instance = new UserCollection;
  }

  return instance;
}

void model::UserCollection::addUser(const model::User& user) {
  users.push_back(user);
}

model::User model::UserCollection::getUser(unsigned int index) {
  return users.at(index);
}

unsigned long model::UserCollection::getSize() {
  return users.size();
}

void model::UserCollection::deleteUser(unsigned int index) {
  users.erase(users.begin() + index);
}

long model::UserCollection::findUser(const model::User& user) {
  long index = -1;
  auto it = std::find(users.begin(), users.end(), user);
  if (it != users.end()) {
    index = std::distance(users.begin(), it);
  }

  return index;
}
