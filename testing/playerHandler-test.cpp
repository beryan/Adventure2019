//
// Created by louis on 26/01/19.
//

#include "lib/gtest/gtest.h"
#include "lib/gmock/gmock.h"
#include "PlayerHandler.h"
#include "Response.h"
#include "Server.h"

using model::PlayerHandler;
using model::Response;
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
 *  9.  Verify registration state is cleared on failure
 *  10.  Logged in after registering
 *  11. Prevent registration with taken username on username input
 *  12. Prevent registration with taken username on second password input
 *  13. Remove appropriate 'reg' states if client disconnects while in registration process
 *  14. Start login and prompt for username
 *  15. Login prompts for password after entering username
 *  16. Enter invalid credentials in login
 *  17. Enter valid credentials in login
 *  18. Verify login state is cleared on failure
 *  19. Logout other client if same Player logged in by a client
 *  20. Remove appropriate 'login' states if client disconnects while in login process
 */
constexpr Connection CLIENT_A = {100};
constexpr Connection CLIENT_B = {200};

constexpr unsigned short EXPECTED_MIN_PASSWORD_LENGTH = 4;
constexpr unsigned short EXPECTED_MAX_USERNAME_AND_PASSWORD_LENGTH = 16;

constexpr auto VALID_LENGTH_STRING = "Valid Input";
constexpr auto LONG_LENGTH_STRING = "Very very very long input";
constexpr auto SHORT_LENGTH_STRING = "SLS";


TEST(PlayerHandlerTestSuite, canStartRegistration) {
    PlayerHandler playerHandler{};

    auto result = playerHandler.processRegistration(CLIENT_A);

    std::ostringstream expect;
    expect << "\n"
           << "Register\n"
           << "--------\n"
           << "Enter your username (maximum of " << EXPECTED_MAX_USERNAME_AND_PASSWORD_LENGTH << " characters)\n";

    EXPECT_EQ(expect.str(), result);
    EXPECT_TRUE(playerHandler.isRegistering(CLIENT_A));
}

TEST(PlayerHandlerTestSuite, canPreventLongUsername) {
    PlayerHandler playerHandler{};

    playerHandler.processRegistration(CLIENT_A);
    auto result = playerHandler.processRegistration(CLIENT_A, LONG_LENGTH_STRING);

    EXPECT_EQ("The username you entered is too long. Registration process cancelled.\n", result);
}

TEST(PlayerHandlerTestSuite, canUseValidUsername) {
    PlayerHandler playerHandler{};

    playerHandler.processRegistration(CLIENT_A);
    auto result = playerHandler.processRegistration(CLIENT_A, VALID_LENGTH_STRING);

    std::ostringstream expect;
    expect << VALID_LENGTH_STRING << "\n"
           << "Enter your password (minimum of " << EXPECTED_MIN_PASSWORD_LENGTH << " characters,"
           << " maximum of " << EXPECTED_MAX_USERNAME_AND_PASSWORD_LENGTH << " characters)\n";

    EXPECT_EQ(expect.str(), result);
}

TEST(PlayerHandlerTestSuite, canPreventShortPassword) {
    PlayerHandler playerHandler{};

    playerHandler.processRegistration(CLIENT_A);
    playerHandler.processRegistration(CLIENT_A, VALID_LENGTH_STRING);
    auto result = playerHandler.processRegistration(CLIENT_A, SHORT_LENGTH_STRING);

    EXPECT_EQ("The password you entered is too short. Registration process cancelled.\n", result);
}

TEST(PlayerHandlerTestSuite, canPreventLongPassword) {
    PlayerHandler playerHandler{};

    playerHandler.processRegistration(CLIENT_A);
    playerHandler.processRegistration(CLIENT_A, VALID_LENGTH_STRING);
    auto result = playerHandler.processRegistration(CLIENT_A, LONG_LENGTH_STRING);

    EXPECT_EQ("The password you entered is too long. Registration process cancelled.\n", result);
}

TEST(PlayerHandlerTestSuite, canUseValidPassword) {
    PlayerHandler playerHandler{};

    playerHandler.processRegistration(CLIENT_A);
    playerHandler.processRegistration(CLIENT_A, VALID_LENGTH_STRING);
    auto result =  playerHandler.processRegistration(CLIENT_A, VALID_LENGTH_STRING);

    EXPECT_EQ("Re-enter your password\n", result);
}

TEST(PlayerHandlerTestSuite, canDetectNonMatchingPassword) {
    PlayerHandler playerHandler{};

    playerHandler.processRegistration(CLIENT_A);
    playerHandler.processRegistration(CLIENT_A, VALID_LENGTH_STRING);
    playerHandler.processRegistration(CLIENT_A, VALID_LENGTH_STRING);
    auto result = playerHandler.processRegistration(CLIENT_A, "notMatch");

    EXPECT_EQ("The passwords you entered do not match. Registration process cancelled.\n", result);
}

TEST(PlayerHandlerTestSuite, canRegisterSuccessfully) {
    PlayerHandler playerHandler{};

    playerHandler.processRegistration(CLIENT_A);
    ASSERT_TRUE(playerHandler.isRegistering(CLIENT_A));

    playerHandler.processRegistration(CLIENT_A, VALID_LENGTH_STRING);
    ASSERT_TRUE(playerHandler.isRegistering(CLIENT_A));

    playerHandler.processRegistration(CLIENT_A, VALID_LENGTH_STRING);
    ASSERT_TRUE(playerHandler.isRegistering(CLIENT_A));

    playerHandler.processRegistration(CLIENT_A, VALID_LENGTH_STRING);
    ASSERT_FALSE(playerHandler.isRegistering(CLIENT_A));

    EXPECT_TRUE(playerHandler.isLoggedIn(CLIENT_A));
}

TEST(RegisterTest, RegisterStateClearsOnFail) {
    PlayerHandler playerHandler{};

    // Fail the registration after storing a username and Foobar in state
    playerHandler.processRegistration(CLIENT_A);
    playerHandler.processRegistration(CLIENT_A, "test");
    playerHandler.processRegistration(CLIENT_A, "Foobar");
    playerHandler.processRegistration(CLIENT_A, "f");

    // Perform a registration with valid input and the same client
    playerHandler.processRegistration(CLIENT_A);
    playerHandler.processRegistration(CLIENT_A, "Foobar");
    playerHandler.processRegistration(CLIENT_A, "test");
    playerHandler.processRegistration(CLIENT_A, "test");

    // Registration should be successful (stored password state cleared on failure)
    ASSERT_FALSE(playerHandler.isRegistering(CLIENT_A));
    ASSERT_TRUE(playerHandler.isLoggedIn(CLIENT_A));

    // Client should have intended username (stored username state cleared on failure)
    EXPECT_EQ(playerHandler.getUsernameByClient(CLIENT_A), "Foobar");
}

TEST(PlayerHandlerTestSuite, isLoggedInAfterRegister) {
    PlayerHandler playerHandler{};

    playerHandler.processRegistration(CLIENT_A);
    playerHandler.processRegistration(CLIENT_A, VALID_LENGTH_STRING);
    playerHandler.processRegistration(CLIENT_A, VALID_LENGTH_STRING);
    playerHandler.processRegistration(CLIENT_A, VALID_LENGTH_STRING);

    EXPECT_FALSE(playerHandler.isLoggingIn(CLIENT_A));
    EXPECT_TRUE(playerHandler.isLoggedIn(CLIENT_A));
}

TEST(PlayerHandlerTestSuite, canDetectUsernameTakenOnUsernameEntry) {
    PlayerHandler playerHandler{};

    playerHandler.processRegistration(CLIENT_A);
    playerHandler.processRegistration(CLIENT_A, VALID_LENGTH_STRING);
    playerHandler.processRegistration(CLIENT_A, VALID_LENGTH_STRING);
    playerHandler.processRegistration(CLIENT_A, VALID_LENGTH_STRING);

    playerHandler.processRegistration(CLIENT_B);
    auto result = playerHandler.processRegistration(CLIENT_B, VALID_LENGTH_STRING);

    EXPECT_EQ("The username \"" + static_cast<std::string>(VALID_LENGTH_STRING) + "\" has already been taken, please use a different username.\n", result);
}

TEST(PlayerHandlerTestSuite, canDetectUsernameTakenOnPasswordReEntry) {
    PlayerHandler playerHandler{};

    // Start registration process
    playerHandler.processRegistration(CLIENT_A);
    playerHandler.processRegistration(CLIENT_B);

    // Enters usernames
    playerHandler.processRegistration(CLIENT_A, VALID_LENGTH_STRING);
    playerHandler.processRegistration(CLIENT_B, VALID_LENGTH_STRING);

    // Enters passwords
    playerHandler.processRegistration(CLIENT_A, VALID_LENGTH_STRING);
    playerHandler.processRegistration(CLIENT_B, VALID_LENGTH_STRING);

    // Re-enters passwords
    playerHandler.processRegistration(CLIENT_A, VALID_LENGTH_STRING);
    auto result = playerHandler.processRegistration(CLIENT_B, VALID_LENGTH_STRING);

    EXPECT_EQ("The username \"" + static_cast<std::string>(VALID_LENGTH_STRING) + "\" has already been taken, please use a different username.\n", result);
}

TEST(PlayerHandlerTestSuite, canRemoveClientFromRegisteringOnDisconnect) {
    PlayerHandler playerHandler{};

    playerHandler.processRegistration(CLIENT_A);
    playerHandler.processRegistration(CLIENT_A, VALID_LENGTH_STRING);
    playerHandler.processRegistration(CLIENT_A, VALID_LENGTH_STRING);
    playerHandler.exitRegistration(CLIENT_A);
    auto result = playerHandler.isRegistering(CLIENT_A);

    EXPECT_FALSE(playerHandler.isRegistering(CLIENT_A));
}

TEST(PlayerHandlerTestSuite, canStartLogin) {
    PlayerHandler playerHandler{};

    auto result = playerHandler.processLogin(CLIENT_A);

    EXPECT_EQ("\nLogin\n-----\nEnter your username\n", result);
    EXPECT_TRUE(playerHandler.isLoggingIn(CLIENT_A));
}

TEST(PlayerHandlerTestSuite, canDetectFailedLogin) {
    PlayerHandler playerHandler{};

    playerHandler.processLogin(CLIENT_A);
    playerHandler.processLogin(CLIENT_A, VALID_LENGTH_STRING);
    auto result = playerHandler.processLogin(CLIENT_A, VALID_LENGTH_STRING);

    EXPECT_EQ("Invalid username or password.\n", result);
}

TEST(PlayerHandlerTestSuite, canLogInSuccessfully) {
    PlayerHandler playerHandler{};

    playerHandler.processRegistration(CLIENT_A);
    playerHandler.processRegistration(CLIENT_A, VALID_LENGTH_STRING);
    playerHandler.processRegistration(CLIENT_A, VALID_LENGTH_STRING);
    playerHandler.processRegistration(CLIENT_A, VALID_LENGTH_STRING);
    playerHandler.logoutPlayer(CLIENT_A);

    playerHandler.processLogin(CLIENT_A);
    ASSERT_TRUE(playerHandler.isLoggingIn(CLIENT_A));

    playerHandler.processLogin(CLIENT_A, VALID_LENGTH_STRING);
    ASSERT_TRUE(playerHandler.isLoggingIn(CLIENT_A));

    playerHandler.processLogin(CLIENT_A, VALID_LENGTH_STRING);
    ASSERT_FALSE(playerHandler.isLoggingIn(CLIENT_A));

    EXPECT_TRUE(playerHandler.isLoggedIn(CLIENT_A));
}

TEST(LoginTest, LoginStateClearsOnFail) {
    PlayerHandler playerHandler{};

    // Create an account, then logout
    playerHandler.processRegistration(CLIENT_A);
    playerHandler.processRegistration(CLIENT_A, VALID_LENGTH_STRING);
    playerHandler.processRegistration(CLIENT_A, VALID_LENGTH_STRING);
    playerHandler.processRegistration(CLIENT_A, VALID_LENGTH_STRING);
    playerHandler.logoutPlayer(CLIENT_A);

    // Attempt to login with incorrect username
    playerHandler.processLogin(CLIENT_A);
    playerHandler.processLogin(CLIENT_A, "invalid name");
    playerHandler.processLogin(CLIENT_A, VALID_LENGTH_STRING);

    // Login with correct username and password
    playerHandler.processLogin(CLIENT_A);
    playerHandler.processLogin(CLIENT_A, VALID_LENGTH_STRING);
    playerHandler.processLogin(CLIENT_A, VALID_LENGTH_STRING);

    // Login should be successful (stored username state cleared on failure)
    ASSERT_FALSE(playerHandler.isLoggingIn(CLIENT_A));
    EXPECT_TRUE(playerHandler.isLoggedIn(CLIENT_A));
}

TEST(PlayerHandlerTestSuite, canLogoutClientOnOtherClientLogin) {
    PlayerHandler playerHandler{};

    playerHandler.processRegistration(CLIENT_A);
    playerHandler.processRegistration(CLIENT_A, VALID_LENGTH_STRING);
    playerHandler.processRegistration(CLIENT_A, VALID_LENGTH_STRING);
    playerHandler.processRegistration(CLIENT_A, VALID_LENGTH_STRING);

    playerHandler.processLogin(CLIENT_B);
    playerHandler.processLogin(CLIENT_B, VALID_LENGTH_STRING);
    playerHandler.processLogin(CLIENT_B, VALID_LENGTH_STRING);
    std::deque<Response> results = {};
    playerHandler.notifyBootedClients(results);

    EXPECT_EQ(CLIENT_A, results.front().client);
    EXPECT_EQ("You have been logged out due to being logged in elsewhere.\n", results.front().message);
    EXPECT_FALSE(playerHandler.isLoggedIn(CLIENT_A));
    EXPECT_TRUE(playerHandler.isLoggedIn(CLIENT_B));
}

TEST(PlayerHandlerTestSuite, canRemoveClientFromLoginOnDisconnect) {
    PlayerHandler playerHandler{};

    playerHandler.processLogin(CLIENT_A);
    playerHandler.processLogin(CLIENT_A, VALID_LENGTH_STRING);
    playerHandler.exitLogin(CLIENT_A);

    EXPECT_FALSE(playerHandler.isLoggingIn(CLIENT_A));
}
