//
// Created by Brittany Ryan on 2019-02-01.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "World.h"

using model::World;
using model::Area;

namespace {

    TEST(WorldTestSuite, canConstructWorld) {
        Area expected_area1{"area1"};
        Area expected_area2{"area2"};
        std::vector<Area> areas = {expected_area1, expected_area2};
        size_t expected_size = areas.size();

        World world{areas};
        EXPECT_EQ(expected_size, world.getAreas().size());
        EXPECT_EQ(expected_area1, world.getAreas().at(0));
        EXPECT_EQ(expected_area2, world.getAreas().at(1));
    }

    TEST(WorldTestSuite, canAddArea) {
        World world{};
        size_t expected_size = world.getAreas().size() + 1;
        Area expected_area{"area"};

        world.addArea(expected_area);
        EXPECT_EQ(expected_size, world.getAreas().size());
        EXPECT_EQ(expected_area, world.getAreas().at(0));
    }

}
