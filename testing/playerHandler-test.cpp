//
// Created by louis on 26/01/19.
//

#include "lib/gtest/gtest.h"
#include "lib/gmock/gmock.h"
#include "PlayerHandler.h"
#include "Response.h"

using model::PlayerHandler;
using model::Response;

/*
 *  Cases to test:
 *  1.  Start register and prompt for username
 *  2.  Enter long username
 *  3.  Enter valid username
 *  4.  Enter short password
 *  5.  Enter long password
 *  6.  Enter valid password
 *  7.  Enter non-matching re-entered-password
 *  8.  Enter matching re-entered-password / Successful registration
 *  9.  Can log out after registering
 *  10. Prevent registration with taken username on username input
 *  11. Prevent registration with taken username on second password input
 *  12. Start login and prompt for username
 *  13. Login prompts for password after entering username
 *  14. Enter invalid credentials in login
 *  15. Enter valid credentials in login
 *  16. Logout other client if same Player logged in by a client
 */
uintptr_t clientIdA = 100;
uintptr_t clientIdB = 200;

const std::string validLengthString = "1234567890123456";
const std::string longLengthString  = "12345678901234567";
const std::string shortLengthString = "12345";

TEST(RegisterTest, StartRegistration) {
    PlayerHandler playerHandler{};

    auto result = playerHandler.startRegistration(clientIdA);

    EXPECT_EQ("\nRegister\n--------\nEnter your username (maximum of 16 characters)\n", result);
}

TEST(RegisterTest, LongUsername) {
    PlayerHandler playerHandler{};

    playerHandler.startRegistration(clientIdA);
    auto result = playerHandler.processRegistration(clientIdA, longLengthString);

    EXPECT_EQ("The username you entered is too long. Registration process cancelled.\n", result);
}

TEST(RegisterTest, ValidUsername) {
    PlayerHandler playerHandler{};

    playerHandler.startRegistration(clientIdA);
    auto result = playerHandler.processRegistration(clientIdA, validLengthString);

    EXPECT_EQ(validLengthString + "\nEnter your password (minimum of 6 characters, maximum of 16 characters)\n", result);
}

TEST(RegisterTest, ShortPassword) {
    PlayerHandler playerHandler{};

    playerHandler.startRegistration(clientIdA);
    playerHandler.processRegistration(clientIdA, validLengthString);
    auto result = playerHandler.processRegistration(clientIdA, shortLengthString);

    EXPECT_EQ("The password you entered is too short. Registration process cancelled.\n", result);
}

TEST(RegisterTest, LongPassword) {
    PlayerHandler playerHandler{};

    playerHandler.startRegistration(clientIdA);
    playerHandler.processRegistration(clientIdA, validLengthString);
    auto result = playerHandler.processRegistration(clientIdA, longLengthString);

    EXPECT_EQ("The password you entered is too long. Registration process cancelled.\n", result);
}

TEST(RegisterTest, ValidPassword) {
    PlayerHandler playerHandler{};

    playerHandler.startRegistration(clientIdA);
    playerHandler.processRegistration(clientIdA, validLengthString);
    auto result =  playerHandler.processRegistration(clientIdA, validLengthString);

    EXPECT_EQ("Re-enter your password\n", result);
}

TEST(RegisterTest, NonMatchingPassword) {
    PlayerHandler playerHandler{};

    playerHandler.startRegistration(clientIdA);
    playerHandler.processRegistration(clientIdA, validLengthString);
    playerHandler.processRegistration(clientIdA, validLengthString);
    auto result = playerHandler.processRegistration(clientIdA, "notMatch");

    EXPECT_EQ("The passwords you entered do not match. Registration process cancelled.\n", result);
}

TEST(RegisterTest, SuccessfulRegistration) {
    PlayerHandler playerHandler{};

    playerHandler.startRegistration(clientIdA);
    playerHandler.processRegistration(clientIdA, validLengthString);
    playerHandler.processRegistration(clientIdA, validLengthString);
    auto result = playerHandler.processRegistration(clientIdA, validLengthString);

    EXPECT_EQ("Your account has been successfully registered and you are now logged in.\n\n", result);
}

TEST(RegisterTest, LoggedInAfterRegister) {
    PlayerHandler playerHandler{};

    playerHandler.startRegistration(clientIdA);
    playerHandler.processRegistration(clientIdA, validLengthString);
    playerHandler.processRegistration(clientIdA, validLengthString);
    playerHandler.processRegistration(clientIdA, validLengthString);
    auto result = playerHandler.logoutPlayer(clientIdA);

    EXPECT_EQ("Logged out successfully.\n", result);
}

TEST(RegisterTest, UsernameTakenOnUsernameEntry) {
    PlayerHandler playerHandler{};

    playerHandler.startRegistration(clientIdA);
    playerHandler.processRegistration(clientIdA, validLengthString);
    playerHandler.processRegistration(clientIdA, validLengthString);
    playerHandler.processRegistration(clientIdA, validLengthString);

    playerHandler.startRegistration(clientIdB);
    auto result = playerHandler.processRegistration(clientIdB, validLengthString);

    EXPECT_EQ("The username \"" + validLengthString + "\" has already been taken, please use a different username.\n", result);
}

TEST(RegisterTest, UsernameTakenOnPasswordReEntry) {
    PlayerHandler playerHandler{};

    playerHandler.startRegistration(clientIdA);
    playerHandler.startRegistration(clientIdB);

    // Enters usernames
    playerHandler.processRegistration(clientIdA, validLengthString);
    playerHandler.processRegistration(clientIdB, validLengthString);

    // Enters passwords
    playerHandler.processRegistration(clientIdA, validLengthString);
    playerHandler.processRegistration(clientIdB, validLengthString);

    // Re-enters passwords
    playerHandler.processRegistration(clientIdA, validLengthString);
    auto result = playerHandler.processRegistration(clientIdB, validLengthString);

    EXPECT_EQ("The username \"" + validLengthString + "\" has already been taken, please use a different username.\n", result);
}

TEST(LoginTest, StartLogin) {
    PlayerHandler playerHandler{};

    auto result = playerHandler.startLogin(clientIdA);

    EXPECT_EQ("\nLogin\n-----\nEnter your username\n", result);
}

TEST(LoginTest, FailedLogin) {
    PlayerHandler playerHandler{};

    playerHandler.startLogin(clientIdA);
    playerHandler.processLogin(clientIdA, validLengthString);
    auto result = playerHandler.processLogin(clientIdA, validLengthString);

    EXPECT_EQ("Invalid username or password.\n", result);
}

TEST(LoginTest, SuccessfulLogin) {
    PlayerHandler playerHandler{};

    playerHandler.startRegistration(clientIdA);
    playerHandler.processRegistration(clientIdA, validLengthString);
    playerHandler.processRegistration(clientIdA, validLengthString);
    playerHandler.processRegistration(clientIdA, validLengthString);
    playerHandler.logoutPlayer(clientIdA);

    playerHandler.startLogin(clientIdA);
    playerHandler.processLogin(clientIdA, validLengthString);
    auto result = playerHandler.processLogin(clientIdA, validLengthString);

    EXPECT_EQ("Logged in successfully!\n", result);
}

TEST(LoginTest, LogoutClientOnOtherClientLogin) {
    PlayerHandler playerHandler{};

    playerHandler.startRegistration(clientIdA);
    playerHandler.processRegistration(clientIdA, validLengthString);
    playerHandler.processRegistration(clientIdA, validLengthString);
    playerHandler.processRegistration(clientIdA, validLengthString);

    playerHandler.startLogin(clientIdB);
    playerHandler.processLogin(clientIdB, validLengthString);
    playerHandler.processLogin(clientIdB, validLengthString);
    std::deque<Response> results = {};
    playerHandler.notifyBootedClients(results);

    EXPECT_EQ(clientIdA, results.front().clientId);
    EXPECT_EQ("You have been logged out due to being logged in elsewhere.\n", results.front().message);
}
