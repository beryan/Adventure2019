//
// Created by Stephen Wanhella on 2019-02-14.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "CommandHandler.h"

namespace {
    TEST(CommandHandlerTestSuite, canGetCommandsFromStrings) {
        model::CommandHandler commandHandler;
        EXPECT_EQ(model::Command::HELP, commandHandler.getCommand("help"));
        EXPECT_EQ(model::Command::LOGIN, commandHandler.getCommand("login"));
        EXPECT_EQ(model::Command::LOGOUT, commandHandler.getCommand("logout"));
        EXPECT_EQ(model::Command::LOOK, commandHandler.getCommand("look"));
        EXPECT_EQ(model::Command::MOVE, commandHandler.getCommand("move"));
        EXPECT_EQ(model::Command::QUIT, commandHandler.getCommand("quit"));
        EXPECT_EQ(model::Command::REGISTER, commandHandler.getCommand("register"));
        EXPECT_EQ(model::Command::SAY, commandHandler.getCommand("say"));
        EXPECT_EQ(model::Command::SHUTDOWN, commandHandler.getCommand("shutdown"));
        EXPECT_EQ(model::Command::TELL, commandHandler.getCommand("tell"));
        EXPECT_EQ(model::Command::YELL, commandHandler.getCommand("yell"));
    }

    TEST(CommandHandlerTestSuite, canGetInvalidCommand) {
        model::CommandHandler commandHandler;
        EXPECT_EQ(model::Command::INVALID_COMMAND, commandHandler.getCommand("asdfasdfasdf"));
    }

    TEST(CommandHandlerTestSuite, canGetAliasedCommands) {
        model::CommandHandler commandHandler;
        std::string testUser = "testuser";
        EXPECT_EQ(model::Command::HELP, commandHandler.getCommand("test_help", testUser));
        EXPECT_EQ(model::Command::LOGIN, commandHandler.getCommand("test_login", testUser));
        EXPECT_EQ(model::Command::LOGOUT, commandHandler.getCommand("test_logout", testUser));
        EXPECT_EQ(model::Command::LOOK, commandHandler.getCommand("test_look", testUser));
        EXPECT_EQ(model::Command::MOVE, commandHandler.getCommand("test_move", testUser));
        EXPECT_EQ(model::Command::QUIT, commandHandler.getCommand("test_quit", testUser));
        EXPECT_EQ(model::Command::REGISTER, commandHandler.getCommand("test_register", testUser));
        EXPECT_EQ(model::Command::SAY, commandHandler.getCommand("test_say", testUser));
        EXPECT_EQ(model::Command::SHUTDOWN, commandHandler.getCommand("test_shutdown", testUser));
        EXPECT_EQ(model::Command::TELL, commandHandler.getCommand("test_tell", testUser));
        EXPECT_EQ(model::Command::YELL, commandHandler.getCommand("test_yell", testUser));
    }

    TEST(CommandHandlerTestSuite, aliasedCommandsFallBackToDefault) {
        model::CommandHandler commandHandler;
        std::string testUser = "testuser_no_aliases";
        EXPECT_EQ(model::Command::HELP, commandHandler.getCommand("help", testUser));
        EXPECT_EQ(model::Command::LOGIN, commandHandler.getCommand("login", testUser));
        EXPECT_EQ(model::Command::LOGOUT, commandHandler.getCommand("logout", testUser));
        EXPECT_EQ(model::Command::LOOK, commandHandler.getCommand("look", testUser));
        EXPECT_EQ(model::Command::MOVE, commandHandler.getCommand("move", testUser));
        EXPECT_EQ(model::Command::QUIT, commandHandler.getCommand("quit", testUser));
        EXPECT_EQ(model::Command::REGISTER, commandHandler.getCommand("register", testUser));
        EXPECT_EQ(model::Command::SAY, commandHandler.getCommand("say", testUser));
        EXPECT_EQ(model::Command::SHUTDOWN, commandHandler.getCommand("shutdown", testUser));
        EXPECT_EQ(model::Command::TELL, commandHandler.getCommand("tell", testUser));
        EXPECT_EQ(model::Command::YELL, commandHandler.getCommand("yell", testUser));
    }
}
