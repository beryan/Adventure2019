//
// Created by Stephen Wanhella on 2019-01-29.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "PropertiesManager.h"
#include <string>

using model::PropertiesManager;

namespace {
  TEST(PropertiesManagerTestSuite, canGetStringProperty) {
    std::string expected = "my test string";
    std::string key = "test_string_property";
    std::string res;
    bool hasProp = PropertiesManager::getStringProperty(key, res);
    EXPECT_TRUE(hasProp);
    EXPECT_EQ(expected, res);
  }

  TEST(PropertiesManagerTestSuite, canGetIntProperty) {
    int expected = 420;
    int result;
    std::string key = "test_int_property";
    bool hasProp = PropertiesManager::getIntProperty(key, result);
    EXPECT_TRUE(hasProp);
    EXPECT_EQ(expected, result);
  }

  TEST(PropertiesManagerTestSuite, canGetBoolProperty) {
    bool expected = true;
    std::string key = "test_bool_property";
    bool result;
    bool hasProp = PropertiesManager::getBoolProperty(key, result);
    EXPECT_TRUE(hasProp);
    EXPECT_EQ(expected, result);
  }

  TEST(PropertiesManagerTestSuite, canNotGetEmptyStringProperty) {
    std::string key = "test_not_exist";
    std::string result;
    bool hasProp = PropertiesManager::getStringProperty(key, result);
    EXPECT_FALSE(hasProp);
  }

  TEST(PropertiesManagerTestSuite, canNotGetEmptyIntProperty) {
    std::string key = "test_not_exist";
    int result;
    bool hasProp = PropertiesManager::getIntProperty(key, result);
    EXPECT_FALSE(hasProp);
  }

  TEST(PropertiesManagerTestSuite, canNotGetEmptyBoolProperty) {
    std::string key = "test_not_exist";
    bool result;
    bool hasProp = PropertiesManager::getBoolProperty(key, result);
    EXPECT_FALSE(hasProp);
  }
}
