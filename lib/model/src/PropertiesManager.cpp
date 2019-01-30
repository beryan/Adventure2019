//
// Created by Stephen Wanhella on 2019-01-29.
//

#include <PropertiesManager.h>
#include <exception>
#include <fstream>
#include <iterator>
#include "PropertiesManager.h"
#include "json.hpp"
#include <iostream>
#include <vector>

using model::PropertiesManager;
using json = nlohmann::json;
const std::string PROPERTIES_PATH = "lib/data/properties.json";

bool PropertiesManager::getStringProperty(std::string propertyName, std::string &result) {
  std::ifstream ifs(PROPERTIES_PATH);

  json t = json::parse(ifs);

  auto iterator = t.find(propertyName);
  if (iterator != t.end()) {
    result = *iterator;
    return true;
  }

  return false;
}

bool PropertiesManager::getIntProperty(std::string propertyName, int &result) {
  std::ifstream ifs(PROPERTIES_PATH);

  json t = json::parse(ifs);

  auto iterator = t.find(propertyName);
  if (iterator != t.end()) {
    result = *iterator;
    return true;
  }

  return false;
}

bool PropertiesManager::getBoolProperty(std::string propertyName, bool &result) {
  std::ifstream ifs(PROPERTIES_PATH);

  json t = json::parse(ifs);

  auto iterator = t.find(propertyName);
  if (iterator != t.end()) {
    result = *iterator;
    return true;
  }

  return false;
}