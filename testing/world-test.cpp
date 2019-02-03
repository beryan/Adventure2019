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
        std::vector<Area> expected_areas = {{"area1"},{"area2"}};
        World world{expected_areas};

        EXPECT_EQ(world.getAreas().size(), expected_areas.size());
    }

    TEST(WorldTestSuite, canAddArea) {
      World world{};
      size_t sizeBefore = world.getAreas().size();

      world.addArea({});
      EXPECT_EQ(world.getAreas().size(), sizeBefore + 1);
    }

}
