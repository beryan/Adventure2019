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

        EXPECT_EQ(expected_name, area.getName());
    }

    TEST(AreaTestSuite, canAddRoom) {
        Area area{};
        size_t expected_size = area.getRooms().size() + 1;

        area.addRoom({});
        EXPECT_EQ(expected_size, area.getRooms().size());
    }

    TEST(AreaTestSuite, canCompareArea) {
        Area area1{};
        Area area2{};
        area1.setName("name");

        area2.setName("name");
        EXPECT_TRUE(area1 == area2);

        area2.setName("other name");
        EXPECT_FALSE(area1 == area2);
    }

}
