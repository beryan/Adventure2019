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
        std::vector<Area> expected_areas;
        expected_areas.push_back({"area1"});
        expected_areas.push_back({"area2"});
        World world{expected_areas};

        EXPECT_EQ(world.getAreas().size(), expected_areas.size());
    }

}
