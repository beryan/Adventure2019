//
// Created by Brittany Ryan on 2019-02-01.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Area.h"

using model::Area;

namespace {

    TEST(AreaTestSuite, canConstructArea) {
        std::string expected_name = "name";
        Area area{expected_name};

        EXPECT_EQ(area.getName(), expected_name);
    }

    TEST(AreaTestSuite, canAddRoom) {
      Area area{};
      size_t sizeBefore = area.getRooms().size();

      area.addRoom({});
      EXPECT_EQ(area.getRooms().size(), sizeBefore + 1);
    }

}
