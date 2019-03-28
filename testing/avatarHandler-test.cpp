//
// Created by louis on 19/03/19.
//

#include "lib/gtest/gtest.h"
#include "lib/gmock/gmock.h"
#include <boost/algorithm/string/case_conv.hpp>
#include "AvatarHandler.h"
#include "Server.h"

using handler::AvatarHandler;

/*
 *  Cases to test:
 *  1.  Start avatar creation and prompt gender
 *  2.  Can parse valid gender selection input correctly
 *  3.  Can prompt for gender re-entry on invalid input
 *  4.  Can parse valid race selection input correctly
 *  5.  Can accept first trait input
 *  6.  Can accept second trait input
 *  7.  Can set player avatar with 'Yes' in confirmation prompt
 *  8.  Can restart avatar creation with 'No' in confirmation prompt
 *  9.  Can prompt for confirmation re-entry on invalid input
 */

constexpr Connection CLIENT = {100};
constexpr auto VALID_PASSWORD_STRING = "Valid Input";


TEST(AvatarHandlerTestSuite, canStartAvatarCreation) {
    AccountHandler accountHandler{};
    AvatarHandler avatarHandler{accountHandler};

    ASSERT_FALSE(avatarHandler.isCreatingAvatar(CLIENT));

    auto result = avatarHandler.processCreation(CLIENT);
    std::ostringstream expect;
    expect << "\n"
           << "Avatar Creation\n"
           << "---------------\n"
           << "Enter the gender of your character: [Male/Female]\n";

    EXPECT_EQ(expect.str(), result);
    EXPECT_TRUE(avatarHandler.isCreatingAvatar(CLIENT));
}


TEST(AvatarHandlerTestSuite, canParseGenderCorrectly) {
    AccountHandler accountHandler{};
    AvatarHandler avatarHandler{accountHandler};

    ASSERT_FALSE(avatarHandler.isCreatingAvatar(CLIENT));

    std::vector<std::string> validMaleInput = {"m", "male", "M", "MALE", "mAlE"};
    std::vector<std::string> validFemaleInput = {"f", "female", "F", "FEMALE", "fEmAlE"};

    for (const auto &input : validMaleInput) {
        avatarHandler.processCreation(CLIENT);
        auto result = avatarHandler.processCreation(CLIENT, input);
        std::ostringstream expect;
        expect << "male\n"
               << "Enter the race of your character: [Human/Dwarf/Elf/Orc]\n";

        ASSERT_EQ(expect.str(), result);
        ASSERT_TRUE(avatarHandler.isCreatingAvatar(CLIENT));

        avatarHandler.exitCreation(CLIENT);
        ASSERT_FALSE(avatarHandler.isCreatingAvatar(CLIENT));
    }

    for (const auto &input : validFemaleInput) {
        avatarHandler.processCreation(CLIENT);
        auto result = avatarHandler.processCreation(CLIENT, input);
        std::ostringstream expect;
        expect << "female\n"
               << "Enter the race of your character: [Human/Dwarf/Elf/Orc]\n";

        ASSERT_EQ(expect.str(), result);
        ASSERT_TRUE(avatarHandler.isCreatingAvatar(CLIENT));

        avatarHandler.exitCreation(CLIENT);
        ASSERT_FALSE(avatarHandler.isCreatingAvatar(CLIENT));
    }
}


TEST(AvatarHandlerTestSuite, canPromptRetryOnInvalidGender) {
    AccountHandler accountHandler{};
    AvatarHandler avatarHandler{accountHandler};

    avatarHandler.processCreation(CLIENT);

    auto result = avatarHandler.processCreation(CLIENT, "invalid");
    std::ostringstream expect;
    expect << "Invalid input, please try again.\n"
           << "Enter the gender of your character: [Male/Female]\n";

    EXPECT_EQ(expect.str(), result);
    EXPECT_TRUE(avatarHandler.isCreatingAvatar(CLIENT));
}


TEST(AvatarHandlerTestSuite, canParseRaceCorrectly) {
    AccountHandler accountHandler{};
    AvatarHandler avatarHandler{accountHandler};
    std::vector<std::string> validInput = {"Human", "elf", "DWARF", "orc"};

    for (const auto &input : validInput) {
        avatarHandler.processCreation(CLIENT);
        avatarHandler.processCreation(CLIENT, "m");
        auto result = avatarHandler.processCreation(CLIENT, input);
        std::ostringstream expect;
        expect << boost::algorithm::to_lower_copy(input) << "\n"
               << "Enter a trait of your character:\n";

        ASSERT_EQ(expect.str(), result);
        ASSERT_TRUE(avatarHandler.isCreatingAvatar(CLIENT));

        avatarHandler.exitCreation(CLIENT);
        ASSERT_FALSE(avatarHandler.isCreatingAvatar(CLIENT));
    }
}


TEST(AvatarHandlerTestSuite, canPromptRetryOnInvalidRace) {
    AccountHandler accountHandler{};
    AvatarHandler avatarHandler{accountHandler};

    avatarHandler.processCreation(CLIENT);
    avatarHandler.processCreation(CLIENT, "m");

    auto result = avatarHandler.processCreation(CLIENT, "invalid");
    std::ostringstream expect;
    expect << "Invalid input, please try again.\n"
           << "Enter the race of your character: [Human/Dwarf/Elf/Orc]\n";

    EXPECT_EQ(expect.str(), result);
    EXPECT_TRUE(avatarHandler.isCreatingAvatar(CLIENT));
}


TEST(AvatarHandlerTestSuite, canAcceptFirstTraitInput) {
    AccountHandler accountHandler{};
    AvatarHandler avatarHandler{accountHandler};

    avatarHandler.processCreation(CLIENT);
    avatarHandler.processCreation(CLIENT, "m");
    avatarHandler.processCreation(CLIENT, "human");

    auto result = avatarHandler.processCreation(CLIENT, "sHorT");
    std::ostringstream expect;
    expect << "short\n"
           << "Enter a second trait of your character:\n";

    EXPECT_EQ(expect.str(), result);
    EXPECT_TRUE(avatarHandler.isCreatingAvatar(CLIENT));
}


TEST(AvatarHandlerTestSuite, canAcceptSecondTraitInput) {
    AccountHandler accountHandler{};
    AvatarHandler avatarHandler{accountHandler};

    avatarHandler.processCreation(CLIENT);
    avatarHandler.processCreation(CLIENT, "m");
    avatarHandler.processCreation(CLIENT, "human");
    avatarHandler.processCreation(CLIENT, "sHoRt");

    auto result = avatarHandler.processCreation(CLIENT, "MUSCULAR");
    std::ostringstream expect;
    expect << "muscular\n"
           << "\n"
           << "Gender: "  << "male\n"
           << "Race: "    << "human\n"
           << "Trait 1: " << "short\n"
           << "Trait 2: " << "muscular\n"
           << "Is the above description of your character correct? [Yes/No]\n";

    EXPECT_EQ(expect.str(), result);
    EXPECT_TRUE(avatarHandler.isCreatingAvatar(CLIENT));
}


TEST(AvatarHandlerTestSuite, canConfirmAvatarCreation) {
    std::vector<std::string> validInput = {"y", "yes", "Y", "Yes", "YES"};
    AccountHandler accountHandler{};
    accountHandler.processRegistration(CLIENT);
    accountHandler.processRegistration(CLIENT, "Able");
    accountHandler.processRegistration(CLIENT, VALID_PASSWORD_STRING);
    accountHandler.processRegistration(CLIENT, VALID_PASSWORD_STRING);

    AvatarHandler avatarHandler{accountHandler};

    for (const auto &input : validInput) {
        auto player = accountHandler.getPlayerByClient(CLIENT);
        player->setAvatar(Avatar{});
        ASSERT_FALSE(player->getAvatar().isDefined());

        avatarHandler.processCreation(CLIENT);
        avatarHandler.processCreation(CLIENT, "m");
        avatarHandler.processCreation(CLIENT, "human");
        avatarHandler.processCreation(CLIENT, "sHoRt");
        avatarHandler.processCreation(CLIENT, "MUSCULAR");
        auto result = avatarHandler.processCreation(CLIENT, input);
        std::ostringstream expect;
        expect << boost::algorithm::to_lower_copy(input) << "\n";
        expect << "Your character has been successfully created!\n";

        ASSERT_EQ(expect.str(), result);
        ASSERT_FALSE(avatarHandler.isCreatingAvatar(CLIENT));
        ASSERT_TRUE(player->getAvatar().isDefined());
    }
}


TEST(AvatarHandlerTestSuite, canRestartCreationOnNoToConfirm) {
    AccountHandler accountHandler{};
    AvatarHandler avatarHandler{accountHandler};

    std::vector<std::string> validInput = {"n", "no", "N", "No", "NO"};

    for (const auto &input : validInput) {
        avatarHandler.processCreation(CLIENT);
        avatarHandler.processCreation(CLIENT, "m");
        avatarHandler.processCreation(CLIENT, "human");
        avatarHandler.processCreation(CLIENT, "sHoRt");
        avatarHandler.processCreation(CLIENT, "MUSCULAR");
        auto result = avatarHandler.processCreation(CLIENT, input);
        std::ostringstream expect;
        expect << boost::algorithm::to_lower_copy(input) << "\n"
               << "Enter the gender of your character: [Male/Female]\n";

        ASSERT_EQ(expect.str(), result);
        ASSERT_TRUE(avatarHandler.isCreatingAvatar(CLIENT));

        avatarHandler.exitCreation(CLIENT);
        ASSERT_FALSE(avatarHandler.isCreatingAvatar(CLIENT));
    }
}

TEST(AvatarHandlerTestSuite, canPromptRetryOnInvalidConfirmInput) {
    AccountHandler accountHandler{};
    AvatarHandler avatarHandler{accountHandler};

    std::vector<std::string> invalidInput = {" ", "1", "s"};

    for (const auto &input : invalidInput) {
        avatarHandler.processCreation(CLIENT);
        avatarHandler.processCreation(CLIENT, "m");
        avatarHandler.processCreation(CLIENT, "human");
        avatarHandler.processCreation(CLIENT, "sHoRt");
        avatarHandler.processCreation(CLIENT, "MUSCULAR");
        auto result = avatarHandler.processCreation(CLIENT, input);
        std::ostringstream expect;
        expect << "Invalid input, please try again.\n"
               << "\n"
               << "Gender: "  << "male\n"
               << "Race: "    << "human\n"
               << "Trait 1: " << "short\n"
               << "Trait 2: " << "muscular\n"
               << "Is the above description of your character correct? [Yes/No]\n";

        ASSERT_EQ(expect.str(), result);
        ASSERT_TRUE(avatarHandler.isCreatingAvatar(CLIENT));

        avatarHandler.exitCreation(CLIENT);
        ASSERT_FALSE(avatarHandler.isCreatingAvatar(CLIENT));
    }
}
