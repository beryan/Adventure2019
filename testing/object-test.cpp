//
// Created by Stephen Wanhella on 2019-02-06.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <Object.h>

namespace {
    class ObjectTestSuite : public ::testing::Test {
    protected:
        model::Slot slot = model::Slot::Head;
        std::string shortDescription = "short";
        std::vector<std::string> longDescription = {"long"};
        std::vector<std::string> keywords = {"keywords"};

        model::Object object{31, shortDescription, longDescription, keywords, slot};
    };

    TEST_F(ObjectTestSuite, canUseNoArgConstructor) {
        model::Object myObject;
        model::ExtraObjectInfo myExtraInfo;
        std::string expected_string;
        unsigned int expected_size = 0;

        EXPECT_EQ(model::Object::DEFAULT_ID, myObject.getId());
        EXPECT_EQ(expected_string, myObject.getShortDescription());
        EXPECT_EQ(expected_size, myObject.getLongDescription().size());
        EXPECT_EQ(expected_size, myObject.getKeywords().size());
        EXPECT_EQ(model::Slot::Misc, myObject.getSlot());
        EXPECT_EQ(myExtraInfo, myObject.getExtraObjectInfo());
    }

    TEST_F(ObjectTestSuite, canUseConstructor) {
        model::ID myId = 42069;
        std::string shortDesc = "A magical skateboard";
        std::vector<std::string> longDesc = {"my", "long", "description", "?"};
        std::vector<std::string> keywords = {"skateboard", "travel", "fast", "cool"};
        model::Slot mySlot = model::Slot::Head;
        model::Object myObject{myId, shortDesc, longDesc, keywords, mySlot};

        model::ExtraObjectInfo emptyExtraInfo;

        EXPECT_EQ(myId, myObject.getId());
        EXPECT_EQ(shortDesc, myObject.getShortDescription());
        EXPECT_EQ(longDesc, myObject.getLongDescription());
        EXPECT_EQ(keywords, myObject.getKeywords());
        EXPECT_EQ(mySlot, myObject.getSlot());
        EXPECT_EQ(emptyExtraInfo, myObject.getExtraObjectInfo());
    }

    TEST_F(ObjectTestSuite, canUseExtraObjectInfoConstructor) {
        model::ID myId = 42069;
        std::string shortDesc = "A magical skateboard";
        std::vector<std::string> longDesc = {"my", "long", "description", "?"};
        std::vector<std::string> keywords = {"skateboard", "travel", "fast", "cool"};
        model::Slot mySlot = model::Slot::Head;
        model::ExtraObjectInfo myExtraInfo{{"asdf"}, {"hello"}};
        model::Object myObject{myId, shortDesc, longDesc, keywords, mySlot, myExtraInfo};

        EXPECT_EQ(myId, myObject.getId());
        EXPECT_EQ(shortDesc, myObject.getShortDescription());
        EXPECT_EQ(longDesc, myObject.getLongDescription());
        EXPECT_EQ(keywords, myObject.getKeywords());
        EXPECT_EQ(mySlot, myObject.getSlot());
        EXPECT_EQ(myExtraInfo, myObject.getExtraObjectInfo());
    }

    TEST_F(ObjectTestSuite, canMakeObjectFromSetters) {
        model::ID myId = 42069;
        std::string shortDesc = "A magical skateboard";
        std::vector<std::string> longDesc = {"my", "long", "description", "?"};
        std::vector<std::string> keywords = {"skateboard", "travel", "fast", "cool"};
        model::Slot mySlot = model::Slot::Head;
        model::ExtraObjectInfo myExtraInfo{{"asdf"}, {"hello"}};
        model::Object myObject{};

        myObject.setId(myId);
        myObject.setShortDescription(shortDesc);
        myObject.setLongDescription(longDesc);
        myObject.setKeywords(keywords);
        myObject.setSlot(mySlot);
        myObject.setExtraObjectInfo(myExtraInfo);

        EXPECT_EQ(myId, myObject.getId());
        EXPECT_EQ(shortDesc, myObject.getShortDescription());
        EXPECT_EQ(longDesc, myObject.getLongDescription());
        EXPECT_EQ(keywords, myObject.getKeywords());
        EXPECT_EQ(mySlot, myObject.getSlot());
        EXPECT_EQ(myExtraInfo, myObject.getExtraObjectInfo());
    }

    TEST_F(ObjectTestSuite, canGetItemSlot) {
        EXPECT_EQ(slot, object.getSlot());
    }

    TEST_F(ObjectTestSuite, canSetItemSlot) {
        object.setSlot(model::Slot::Back);

        EXPECT_EQ(model::Slot::Back, object.getSlot());
    }

    TEST_F(ObjectTestSuite, canGetSlotFromString) {
        object.setSlot(model::getSlotFromString("Feet"));

        ASSERT_EQ(model::Slot::Feet, object.getSlot());

        object.setSlot(model::getSlotFromString("ODKOIJCOIDSC"));

        ASSERT_EQ(model::Slot::Misc, object.getSlot());

        object.setSlot(model::getSlotFromString("WEAPON"));

        EXPECT_EQ(model::Slot::Weapon, object.getSlot());
    }

    TEST_F(ObjectTestSuite, canReturnStringFromSlot) {
        ASSERT_EQ("hello head", "hello " + model::getStringFromSlot(object.getSlot()));

        object.setSlot(Slot::Feet);

        ASSERT_EQ("hello feet", "hello " + model::getStringFromSlot(object.getSlot()));

        ASSERT_EQ("hello misc", "hello " + model::getStringFromSlot(Slot::Count));

    }
}