//
// Created by Stephen Wanhella on 2019-02-06.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "ExtraObjectInfo.h"

namespace {

  TEST(ExtraObjectInfoTestSuite, canUseDefaultConstructor) {
    model::ExtraObjectInfo myInfo;
    int expected_size = 0;
    EXPECT_EQ(expected_size, myInfo.getExtraKeywords().size());
    EXPECT_EQ(expected_size, myInfo.getExtraDescriptions().size());
  }

  TEST(ExtraObjectInfoTestSuite, canUseTwoArgConstructor) {
    std::string myKeyword = "test_keyword";
    std::string myDescription = "test_description";
    model::ExtraObjectInfo myInfo{{myKeyword},
                                  {myDescription}};
    int expected_size = 1;
    EXPECT_EQ(expected_size, myInfo.getExtraKeywords().size());
    EXPECT_EQ(expected_size, myInfo.getExtraDescriptions().size());
    EXPECT_EQ(myKeyword, myInfo.getExtraKeywords().at(0));
    EXPECT_EQ(myDescription, myInfo.getExtraDescriptions().at(0));
  }

  TEST(ExtraObjectInfoTestSuite, canSetExtraKeywords) {
    std::vector<std::string> keywords = {"test1", "test2"};
    model::ExtraObjectInfo myInfo;

    int expected_size1 = 0;
    EXPECT_EQ(expected_size1, myInfo.getExtraKeywords().size());

    unsigned long expected_size2 = keywords.size();
    myInfo.setExtraKeywords(keywords);

    EXPECT_EQ(expected_size2, myInfo.getExtraKeywords().size());

    EXPECT_EQ(keywords, myInfo.getExtraKeywords());
    keywords.emplace_back("test3");

    EXPECT_NE(keywords, myInfo.getExtraKeywords());
  }

  TEST(ExtraObjectInfoTestSuite, canSetExtraDescriptions) {
    std::vector<std::string> descriptions = {"test1", "test2"};
    model::ExtraObjectInfo myInfo;

    int expected_size1 = 0;
    EXPECT_EQ(expected_size1, myInfo.getExtraDescriptions().size());

    unsigned long expected_size2 = descriptions.size();
    myInfo.setExtraDescriptions(descriptions);

    EXPECT_EQ(expected_size2, myInfo.getExtraDescriptions().size());

    EXPECT_EQ(descriptions, myInfo.getExtraDescriptions());
    descriptions.emplace_back("test3");

    EXPECT_NE(descriptions, myInfo.getExtraDescriptions());
  }
}
