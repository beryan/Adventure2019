//
// Created by louis on 26/01/19.
//

#include "lib/gtest/gtest.h"
#include "lib/gmock/gmock.h"
#include "AccountHandler.h"
#include "Server.h"

using handler::AccountHandler;
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
 *  10. Logged in after registering
 *  11. Prevent registration with taken username on username input
 *  12. Prevent registration with taken username on second password input
 *  13. Remove appropriate register states if client disconnects during registration process
 *  14. Start login and prompt for username
 *  15. Login prompts for password after entering username
 *  16. Enter invalid credentials in login
 *  17. Enter valid credentials in login
 *  18. Verify login state is cleared on failure
 *  19. Logout other client if same Player logged in by a client
 *  20. Remove appropriate login states if client disconnects during login process
 */
constexpr Connection CLIENT_A = {100};
constexpr Connection CLIENT_B = {200};

constexpr unsigned short EXPECTED_MIN_PASSWORD_LENGTH = 4;
constexpr unsigned short EXPECTED_MAX_USERNAME_AND_PASSWORD_LENGTH = 16;

constexpr auto VALID_PASSWORD_STRING = "Valid Input";
constexpr auto LONG_LENGTH_STRING = "Very very very long input";
constexpr auto SHORT_LENGTH_STRING = "SLS";


TEST(AccountHandlerTestSuite, canStartRegistration) {
    AccountHandler accountHandler{};

    auto result = accountHandler.processRegistration(CLIENT_A);

    std::ostringstream expected;
    expected << "\n"
             << "Register\n"
             << "--------\n"
             << "Enter your username (maximum of " << EXPECTED_MAX_USERNAME_AND_PASSWORD_LENGTH << " characters)\n";

    EXPECT_EQ(expected.str(), result);
    EXPECT_TRUE(accountHandler.isRegistering(CLIENT_A));
}

TEST(AccountHandlerTestSuite, canPreventLongUsername) {
    AccountHandler accountHandler{};

    accountHandler.processRegistration(CLIENT_A);
    auto result = accountHandler.processRegistration(CLIENT_A, LONG_LENGTH_STRING);

    std::ostringstream expected;
    expected << "The username you entered is too long. Registration process cancelled.\n";

    EXPECT_EQ(expected.str(), result);
}

TEST(AccountHandlerTestSuite, canUseValidUsername) {
    AccountHandler accountHandler{};
    std::string TestUsername = "TestUsername";

    accountHandler.processRegistration(CLIENT_A);
    auto result = accountHandler.processRegistration(CLIENT_A, TestUsername);



    EXPECT_EQ(expected.str(), result);
}

TEST(AccountHandlerTestSuite, canPreventShortPassword) {
    AccountHandler accountHandler{};

    accountHandler.processRegistration(CLIENT_A);
    accountHandler.processRegistration(CLIENT_A, VALID_PASSWORD_STRING);
    auto result = accountHandler.processRegistration(CLIENT_A, SHORT_LENGTH_STRING);

    std::ostringstream expected;
    expected << "The password you entered is too short. Registration process cancelled.\n";

    EXPECT_EQ(expected.str(), result);
}

TEST(AccountHandlerTestSuite, canPreventLongPassword) {
    AccountHandler accountHandler{};

    accountHandler.processRegistration(CLIENT_A);
    accountHandler.processRegistration(CLIENT_A, VALID_PASSWORD_STRING);
    auto result = accountHandler.processRegistration(CLIENT_A, LONG_LENGTH_STRING);

    std::ostringstream expected;
    expected << "The password you entered is too long. Registration process cancelled.\n";

    EXPECT_EQ(expected.str(), result);
}

TEST(AccountHandlerTestSuite, canUseValidPassword) {
    AccountHandler accountHandler{};

    accountHandler.processRegistration(CLIENT_A);
    accountHandler.processRegistration(CLIENT_A, VALID_PASSWORD_STRING);
    auto result =  accountHandler.processRegistration(CLIENT_A, VALID_PASSWORD_STRING);

    std::ostringstream expected;
    expected << "Re-enter your password\n";

    EXPECT_EQ(expected.str(), result);
}

TEST(AccountHandlerTestSuite, canDetectNonMatchingPassword) {
    AccountHandler accountHandler{};

    accountHandler.processRegistration(CLIENT_A);
    accountHandler.processRegistration(CLIENT_A, VALID_PASSWORD_STRING);
    accountHandler.processRegistration(CLIENT_A, VALID_PASSWORD_STRING);
    auto result = accountHandler.processRegistration(CLIENT_A, "notMatch");

    std::ostringstream expected;
    expected << "The passwords you entered do not match. Registration process cancelled.\n";

    EXPECT_EQ(expected.str(), result);
}

TEST(AccountHandlerTestSuite, canRegisterSuccessfully) {
    AccountHandler accountHandler{};
    std::string TestUsername = "canRegister";

    accountHandler.processRegistration(CLIENT_A);
    ASSERT_TRUE(accountHandler.isRegistering(CLIENT_A));

    accountHandler.processRegistration(CLIENT_A, TestUsername);
    ASSERT_TRUE(accountHandler.isRegistering(CLIENT_A));

    accountHandler.processRegistration(CLIENT_A, VALID_PASSWORD_STRING);
    ASSERT_TRUE(accountHandler.isRegistering(CLIENT_A));

    accountHandler.processRegistration(CLIENT_A, VALID_PASSWORD_STRING);
    ASSERT_FALSE(accountHandler.isRegistering(CLIENT_A));

    EXPECT_TRUE(accountHandler.isLoggedIn(CLIENT_A));
}

TEST(AccountHandlerTestSuite, canClearRegisterStateOnFail) {
    AccountHandler accountHandler{};

    // Fail the registration after storing a username and password in state
    accountHandler.processRegistration(CLIENT_A);
    accountHandler.processRegistration(CLIENT_A, "test");
    accountHandler.processRegistration(CLIENT_A, "Foobar");
    accountHandler.processRegistration(CLIENT_A, "f");

    // Perform a registration with valid input and the same client
    accountHandler.processRegistration(CLIENT_A);
    accountHandler.processRegistration(CLIENT_A, "Foobar");
    accountHandler.processRegistration(CLIENT_A, "test");
    accountHandler.processRegistration(CLIENT_A, "test");

    // Registration should be successful (stored password state cleared on failure)
    ASSERT_FALSE(accountHandler.isRegistering(CLIENT_A));
    ASSERT_TRUE(accountHandler.isLoggedIn(CLIENT_A));

    // Client should have intended username (stored username state cleared on failure)
    EXPECT_EQ(accountHandler.getUsernameByClient(CLIENT_A), "Foobar");
}

TEST(AccountHandlerTestSuite, isLoggedInAfterRegister) {
    AccountHandler accountHandler{};
    std::string TestUsername = "isLoggedInAfter";

    accountHandler.processRegistration(CLIENT_A);
    accountHandler.processRegistration(CLIENT_A, TestUsername);
    accountHandler.processRegistration(CLIENT_A, VALID_PASSWORD_STRING);
    accountHandler.processRegistration(CLIENT_A, VALID_PASSWORD_STRING);

    EXPECT_FALSE(accountHandler.isLoggingIn(CLIENT_A));
    EXPECT_TRUE(accountHandler.isLoggedIn(CLIENT_A));
}

TEST(AccountHandlerTestSuite, canDetectUsernameTakenOnUsernameEntry) {
    AccountHandler accountHandler{};
    std::string TestUsername = "TakenUsername";

    accountHandler.processRegistration(CLIENT_A);
    accountHandler.processRegistration(CLIENT_A, TestUsername);
    accountHandler.processRegistration(CLIENT_A, VALID_PASSWORD_STRING);
    accountHandler.processRegistration(CLIENT_A, VALID_PASSWORD_STRING);

    accountHandler.processRegistration(CLIENT_B);
    auto result = accountHandler.processRegistration(CLIENT_B, TestUsername);

    std::ostringstream expected;
    expected << "The username \"" << static_cast<std::string>(TestUsername)
             << "\" has already been taken, please use a different username.\n";

    EXPECT_EQ(expected.str(), result);
}

TEST(AccountHandlerTestSuite, canDetectUsernameTakenOnPasswordReEntry) {
    AccountHandler accountHandler{};
    std::string TestUsername = "TakenUsername2";

    // Start registration process
    accountHandler.processRegistration(CLIENT_A);
    accountHandler.processRegistration(CLIENT_B);

    // Enters usernames
    accountHandler.processRegistration(CLIENT_A, TestUsername);
    accountHandler.processRegistration(CLIENT_B, TestUsername);

    // Enter passwords
    accountHandler.processRegistration(CLIENT_A, VALID_PASSWORD_STRING);
    accountHandler.processRegistration(CLIENT_B, VALID_PASSWORD_STRING);

    // Re-enter passwords
    accountHandler.processRegistration(CLIENT_A, VALID_PASSWORD_STRING);
    auto result = accountHandler.processRegistration(CLIENT_B, VALID_PASSWORD_STRING);

    std::ostringstream expected;
    expected << "The username \"" << static_cast<std::string>(TestUsername)
             << "\" has already been taken, please use a different username.\n";

    EXPECT_EQ(expected.str(), result);
}

TEST(AccountHandlerTestSuite, canRemoveClientFromRegisteringOnDisconnect) {
    AccountHandler accountHandler{};

    accountHandler.processRegistration(CLIENT_A);
    accountHandler.processRegistration(CLIENT_A, VALID_PASSWORD_STRING);
    accountHandler.processRegistration(CLIENT_A, VALID_PASSWORD_STRING);
    accountHandler.exitRegistration(CLIENT_A);

    EXPECT_FALSE(accountHandler.isRegistering(CLIENT_A));
}

TEST(AccountHandlerTestSuite, canStartLogin) {
    AccountHandler accountHandler{};

    auto result = accountHandler.processLogin(CLIENT_A);

    std::ostringstream expected;
    expected << "\n"
             << "Login\n"
             << "-----\n"
             << "Enter your username\n";


    EXPECT_EQ(expected.str(), result);
    EXPECT_TRUE(accountHandler.isLoggingIn(CLIENT_A));
}

TEST(AccountHandlerTestSuite, canDetectFailedLogin) {
    AccountHandler accountHandler{};
    std::string TestUsername = "FailedLogin";

    accountHandler.processLogin(CLIENT_A);
    accountHandler.processLogin(CLIENT_A, TestUsername);
    auto result = accountHandler.processLogin(CLIENT_A, VALID_PASSWORD_STRING);

    std::ostringstream expected;
    expected << "Invalid username or password.\n";

    EXPECT_EQ(expected.str(), result);
}

TEST(AccountHandlerTestSuite, canLogInSuccessfully) {
    AccountHandler accountHandler{};
    std::string TestUsername = "canLogin";

    accountHandler.processLogin(CLIENT_A);
    ASSERT_TRUE(accountHandler.isLoggingIn(CLIENT_A));

    accountHandler.processLogin(CLIENT_A, TestUsername);
    ASSERT_TRUE(accountHandler.isLoggingIn(CLIENT_A));

    accountHandler.processLogin(CLIENT_A, VALID_PASSWORD_STRING);
    ASSERT_FALSE(accountHandler.isLoggingIn(CLIENT_A));

    EXPECT_TRUE(accountHandler.isLoggedIn(CLIENT_A));
}

TEST(AccountHandlerTestSuite, LoginStateClearsOnFail) {
    AccountHandler accountHandler{};
    std::string TestUsername = "ClearsOnFail";

    // Attempt to login with incorrect username
    accountHandler.processLogin(CLIENT_A);
    accountHandler.processLogin(CLIENT_A, "invalid name");
    accountHandler.processLogin(CLIENT_A, VALID_PASSWORD_STRING);

    // Login with correct username and password
    accountHandler.processLogin(CLIENT_A);
    accountHandler.processLogin(CLIENT_A, TestUsername);
    accountHandler.processLogin(CLIENT_A, VALID_PASSWORD_STRING);

    // Login should be successful (stored username state cleared on failure)
    ASSERT_FALSE(accountHandler.isLoggingIn(CLIENT_A));
    EXPECT_TRUE(accountHandler.isLoggedIn(CLIENT_A));
}

TEST(AccountHandlerTestSuite, canLogoutClientOnOtherClientLogin) {
    AccountHandler accountHandler{};
    std::string TestUsername = "canLogoutClient";

    accountHandler.processLogin(CLIENT_A);
    accountHandler.processLogin(CLIENT_A, TestUsername);
    accountHandler.processLogin(CLIENT_A, VALID_PASSWORD_STRING);

    accountHandler.processLogin(CLIENT_B);
    accountHandler.processLogin(CLIENT_B, TestUsername);
    accountHandler.processLogin(CLIENT_B, VALID_PASSWORD_STRING);
    std::deque<Message> results = {};
    accountHandler.notifyBootedClients(results);

    std::ostringstream expected;
    expected << "You have been logged out due to being logged in elsewhere.\n";

    EXPECT_EQ(CLIENT_A, results.front().connection);
    EXPECT_EQ(expected.str(), results.front().text);
    EXPECT_FALSE(accountHandler.isLoggedIn(CLIENT_A));
    EXPECT_TRUE(accountHandler.isLoggedIn(CLIENT_B));
}

TEST(AccountHandlerTestSuite, canRemoveClientFromLoginOnDisconnect) {
    AccountHandler accountHandler{};

    accountHandler.processLogin(CLIENT_A);
    accountHandler.processLogin(CLIENT_A, VALID_PASSWORD_STRING);
    accountHandler.exitLogin(CLIENT_A);

    EXPECT_FALSE(accountHandler.isLoggingIn(CLIENT_A));
}
