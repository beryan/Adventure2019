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

    TEST(ObjectTestSuite, canUseConstructor) {
        model::ID myId = 42069;
        std::string name = "Skateboard";
        std::string shortDesc = "This is a magical skateboard";
        std::vector<std::string> longDesc = {"my", "long", "description", "?"};
        std::vector<std::string> keywords = {"skateboard", "travel", "fast", "cool"};
        model::Slot mySlot = model::Slot::Head;
        model::Object myObject{myId, name, shortDesc, longDesc, keywords, mySlot};

        model::ExtraObjectInfo emptyExtraInfo;

        EXPECT_EQ(myId, myObject.getId());
        EXPECT_EQ(name, myObject.getName());
        EXPECT_EQ(shortDesc, myObject.getShortDescription());
        EXPECT_EQ(longDesc, myObject.getLongDescription());
        EXPECT_EQ(keywords, myObject.getKeywords());
        EXPECT_EQ(mySlot, myObject.getSlot());
        EXPECT_EQ(emptyExtraInfo, myObject.getExtraObjectInfo());
    }

    TEST(ObjectTestSuite, canUseExtraObjectInfoConstructor) {
        model::ID myId = 42069;
        std::string name = "Skateboard";
        std::string shortDesc = "This is a magical skateboard";
        std::vector<std::string> longDesc = {"my", "long", "description", "?"};
        std::vector<std::string> keywords = {"skateboard", "travel", "fast", "cool"};
        model::Slot mySlot = model::Slot::Head;
        model::ExtraObjectInfo myExtraInfo{{"asdf"}, {"hello"}};
        model::Object myObject{myId, name, shortDesc, longDesc, keywords, mySlot, myExtraInfo};

        EXPECT_EQ(myId, myObject.getId());
        EXPECT_EQ(name, myObject.getName());
        EXPECT_EQ(shortDesc, myObject.getShortDescription());
        EXPECT_EQ(longDesc, myObject.getLongDescription());
        EXPECT_EQ(keywords, myObject.getKeywords());
        EXPECT_EQ(mySlot, myObject.getSlot());
        EXPECT_EQ(myExtraInfo, myObject.getExtraObjectInfo());
    }

    TEST(ObjectTestSuite, canMakeObjectFromSetters) {
        model::ID myId = 42069;
        std::string name = "Skateboard";
        std::string shortDesc = "This is a magical skateboard";
        std::vector<std::string> longDesc = {"my", "long", "description", "?"};
        std::vector<std::string> keywords = {"skateboard", "travel", "fast", "cool"};
        model::Slot mySlot = model::Slot::Head;
        model::ExtraObjectInfo myExtraInfo{{"asdf"}, {"hello"}};
        model::Object myObject{};

        myObject.setId(myId);
        myObject.setName(name);
        myObject.setShortDescription(shortDesc);
        myObject.setLongDescription(longDesc);
        myObject.setKeywords(keywords);
        myObject.setSlot(mySlot);
        myObject.setExtraObjectInfo(myExtraInfo);

        EXPECT_EQ(myId, myObject.getId());
        EXPECT_EQ(name, myObject.getName());
        EXPECT_EQ(shortDesc, myObject.getShortDescription());
        EXPECT_EQ(longDesc, myObject.getLongDescription());
        EXPECT_EQ(keywords, myObject.getKeywords());
        EXPECT_EQ(mySlot, myObject.getSlot());
        EXPECT_EQ(myExtraInfo, myObject.getExtraObjectInfo());
    }
}