//
// Created by louis on 26/01/19.
//

#include "lib/gtest/gtest.h"
#include "lib/gmock/gmock.h"
#include "PlayerHandler.h"
#include "Server.h"

using model::PlayerHandler;
using networking::Connection;

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
 *  9.  Logged in after registering
 *  10. Prevent registration with taken username on username input
 *  11. Prevent registration with taken username on second password input
 *  12. Remove appropriate 'reg' states if client disconnects while in registration process
 *  13. Start login and prompt for username
 *  14. Login prompts for password after entering username
 *  15. Enter invalid credentials in login
 *  16. Enter valid credentials in login
 *  17. Logout other client if same Player logged in by a client
 *  18. Remove appropriate 'login' states if client disconnects while in login process
 */
const Connection clientIdA = {100};
const Connection clientIdB = {200};

const unsigned short EXPECTED_MIN_PASSWORD_LENGTH = 4;
const unsigned short EXPECTED_MAX_USERNAME_AND_PASSWORD_LENGTH = 16;

const std::string validLengthString = "Valid Input";
const std::string longLengthString  = "Very very very long input";
const std::string shortLengthString = "SLS";


TEST(RegisterTest, StartRegistration) {
    PlayerHandler playerHandler{};

    auto result = playerHandler.processRegistration(clientIdA);

    std::ostringstream expect;
    expect << "\n"
           << "Register\n"
           << "--------\n"
           << "Enter your username (maximum of " << EXPECTED_MAX_USERNAME_AND_PASSWORD_LENGTH << " characters)\n";

    EXPECT_EQ(expect.str(), result);
    EXPECT_EQ(playerHandler.isRegistering(clientIdA), true);
}

TEST(RegisterTest, LongUsername) {
    PlayerHandler playerHandler{};

    playerHandler.processRegistration(clientIdA);
    auto result = playerHandler.processRegistration(clientIdA, longLengthString);

    EXPECT_EQ("The username you entered is too long. Registration process cancelled.\n", result);
}

TEST(RegisterTest, ValidUsername) {
    PlayerHandler playerHandler{};

    playerHandler.processRegistration(clientIdA);
    auto result = playerHandler.processRegistration(clientIdA, validLengthString);

    std::ostringstream expect;
    expect << validLengthString << "\n"
           << "Enter your password (minimum of " << EXPECTED_MIN_PASSWORD_LENGTH << " characters,"
           << " maximum of " << EXPECTED_MAX_USERNAME_AND_PASSWORD_LENGTH << " characters)\n";

    EXPECT_EQ(expect.str(), result);
}

TEST(RegisterTest, ShortPassword) {
    PlayerHandler playerHandler{};

    playerHandler.processRegistration(clientIdA);
    playerHandler.processRegistration(clientIdA, validLengthString);
    auto result = playerHandler.processRegistration(clientIdA, shortLengthString);

    EXPECT_EQ("The password you entered is too short. Registration process cancelled.\n", result);
}

TEST(RegisterTest, LongPassword) {
    PlayerHandler playerHandler{};

    playerHandler.processRegistration(clientIdA);
    playerHandler.processRegistration(clientIdA, validLengthString);
    auto result = playerHandler.processRegistration(clientIdA, longLengthString);

    EXPECT_EQ("The password you entered is too long. Registration process cancelled.\n", result);
}

TEST(RegisterTest, ValidPassword) {
    PlayerHandler playerHandler{};

    playerHandler.processRegistration(clientIdA);
    playerHandler.processRegistration(clientIdA, validLengthString);
    auto result =  playerHandler.processRegistration(clientIdA, validLengthString);

    EXPECT_EQ("Re-enter your password\n", result);
}

TEST(RegisterTest, NonMatchingPassword) {
    PlayerHandler playerHandler{};

    playerHandler.processRegistration(clientIdA);
    playerHandler.processRegistration(clientIdA, validLengthString);
    playerHandler.processRegistration(clientIdA, validLengthString);
    auto result = playerHandler.processRegistration(clientIdA, "notMatch");

    EXPECT_EQ("The passwords you entered do not match. Registration process cancelled.\n", result);
}

TEST(RegisterTest, SuccessfulRegistration) {
    PlayerHandler playerHandler{};

    playerHandler.processRegistration(clientIdA);
    playerHandler.processRegistration(clientIdA, validLengthString);
    playerHandler.processRegistration(clientIdA, validLengthString);
    auto result = playerHandler.processRegistration(clientIdA, validLengthString);

    EXPECT_EQ("Your account has been successfully registered and you are now logged in.\n\n", result);
}

TEST(RegisterTest, LoggedInAfterRegister) {
    PlayerHandler playerHandler{};

    playerHandler.processRegistration(clientIdA);
    playerHandler.processRegistration(clientIdA, validLengthString);
    playerHandler.processRegistration(clientIdA, validLengthString);
    playerHandler.processRegistration(clientIdA, validLengthString);

    EXPECT_EQ(playerHandler.isLoggingIn(clientIdA), false);
    EXPECT_EQ(playerHandler.isLoggedIn(clientIdA), true);
}

TEST(RegisterTest, UsernameTakenOnUsernameEntry) {
    PlayerHandler playerHandler{};

    playerHandler.processRegistration(clientIdA);
    playerHandler.processRegistration(clientIdA, validLengthString);
    playerHandler.processRegistration(clientIdA, validLengthString);
    playerHandler.processRegistration(clientIdA, validLengthString);

    playerHandler.processRegistration(clientIdB);
    auto result = playerHandler.processRegistration(clientIdB, validLengthString);

    EXPECT_EQ("The username \"" + validLengthString + "\" has already been taken, please use a different username.\n", result);
}

TEST(RegisterTest, UsernameTakenOnPasswordReEntry) {
    PlayerHandler playerHandler{};

    // Start registration process
    playerHandler.processRegistration(clientIdA);
    playerHandler.processRegistration(clientIdB);

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

TEST(RegisterTest, RemoveClientFromRegisteringOnDisconnect) {
    PlayerHandler playerHandler{};

    playerHandler.processRegistration(clientIdA);
    playerHandler.processRegistration(clientIdA, validLengthString);
    playerHandler.processRegistration(clientIdA, validLengthString);
    playerHandler.exitRegistration(clientIdA);
    auto result = playerHandler.isRegistering(clientIdA);

    EXPECT_EQ(result, false);
}

TEST(LoginTest, StartLogin) {
    PlayerHandler playerHandler{};

    auto result = playerHandler.processLogin(clientIdA);

    EXPECT_EQ("\nLogin\n-----\nEnter your username\n", result);
    EXPECT_EQ(playerHandler.isLoggingIn(clientIdA), true);
}

TEST(LoginTest, FailedLogin) {
    PlayerHandler playerHandler{};

    playerHandler.processLogin(clientIdA);
    playerHandler.processLogin(clientIdA, validLengthString);
    auto result = playerHandler.processLogin(clientIdA, validLengthString);

    EXPECT_EQ("Invalid username or password.\n", result);
}

TEST(LoginTest, SuccessfulLogin) {
    PlayerHandler playerHandler{};

    playerHandler.processRegistration(clientIdA);
    playerHandler.processRegistration(clientIdA, validLengthString);
    playerHandler.processRegistration(clientIdA, validLengthString);
    playerHandler.processRegistration(clientIdA, validLengthString);
    playerHandler.logoutPlayer(clientIdA);

    playerHandler.processLogin(clientIdA);
    playerHandler.processLogin(clientIdA, validLengthString);
    auto result = playerHandler.processLogin(clientIdA, validLengthString);

    EXPECT_EQ("Logged in successfully!\n", result);
}

TEST(LoginTest, LogoutClientOnOtherClientLogin) {
    PlayerHandler playerHandler{};

    playerHandler.processRegistration(clientIdA);
    playerHandler.processRegistration(clientIdA, validLengthString);
    playerHandler.processRegistration(clientIdA, validLengthString);
    playerHandler.processRegistration(clientIdA, validLengthString);

    playerHandler.processLogin(clientIdB);
    playerHandler.processLogin(clientIdB, validLengthString);
    playerHandler.processLogin(clientIdB, validLengthString);
    std::deque<Message> results = {};
    playerHandler.notifyBootedClients(results);

    EXPECT_EQ(clientIdA, results.front().connection);
    EXPECT_EQ("You have been logged out due to being logged in elsewhere.\n", results.front().text);
    EXPECT_EQ(playerHandler.isLoggedIn(clientIdA), false);
    EXPECT_EQ(playerHandler.isLoggedIn(clientIdB), true);
}

TEST(LoginTest, RemoveClientFromLoginOnDisconnect) {
    PlayerHandler playerHandler{};

    playerHandler.processLogin(clientIdA);
    playerHandler.processLogin(clientIdA, validLengthString);
    playerHandler.exitLogin(clientIdA);

    EXPECT_EQ(playerHandler.isLoggingIn(clientIdA), false);
}
