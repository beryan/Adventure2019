//
// Created by Stephen Wanhella on 2019-02-06.
//

#include <gtest/gtest.h>
#include <Object.h>

namespace {
    TEST(ObjectTestSuite, canUseNoArgConstructor) {
        model::Object myObject;
        model::ExtraObjectInfo myExtraInfo;
        EXPECT_EQ(model::Object::DEFAULT_ID, myObject.getId());
        EXPECT_EQ("", myObject.getName());
        EXPECT_EQ("", myObject.getShortDescription());
        EXPECT_EQ(0, myObject.getLongDescription().size());
        EXPECT_EQ(0, myObject.getKeywords().size());
        EXPECT_EQ(model::Slot::Misc, myObject.getSlot());
        EXPECT_EQ(myExtraInfo, myObject.getExtraObjectInfo());
    }
}