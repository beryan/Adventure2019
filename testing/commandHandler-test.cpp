//
// Created by Stephen Wanhella on 2019-02-14.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "CommandHandler.h"

namespace {
    TEST(CommandHandlerTestSuite, canGetCommandsFromStrings) {
        model::CommandHandler commandHandler;
        EXPECT_EQ(model::Command::Help, commandHandler.getDefaultCommand("help"));
        EXPECT_EQ(model::Command::Login, commandHandler.getDefaultCommand("login"));
        EXPECT_EQ(model::Command::Logout, commandHandler.getDefaultCommand("logout"));
        EXPECT_EQ(model::Command::Look, commandHandler.getDefaultCommand("look"));
        EXPECT_EQ(model::Command::Move, commandHandler.getDefaultCommand("move"));
        EXPECT_EQ(model::Command::Quit, commandHandler.getDefaultCommand("quit"));
        EXPECT_EQ(model::Command::Register, commandHandler.getDefaultCommand("register"));
        EXPECT_EQ(model::Command::Say, commandHandler.getDefaultCommand("say"));
        EXPECT_EQ(model::Command::Shutdown, commandHandler.getDefaultCommand("shutdown"));
        EXPECT_EQ(model::Command::Tell, commandHandler.getDefaultCommand("tell"));
        EXPECT_EQ(model::Command::Yell, commandHandler.getDefaultCommand("yell"));
    }

    TEST(CommandHandlerTestSuite, canGetInvalidCommand) {
        model::CommandHandler commandHandler;
        EXPECT_EQ(model::Command::InvalidCommand, commandHandler.getDefaultCommand("asdfasdfasdf"));
    }

    TEST(CommandHandlerTestSuite, canGetAliasedCommands) {
        model::CommandHandler commandHandler;
        std::string testUser = "testuser";
        EXPECT_EQ(model::Command::Help, commandHandler.getCommandForUser("test_help", testUser));
        EXPECT_EQ(model::Command::Login, commandHandler.getCommandForUser("test_login", testUser));
        EXPECT_EQ(model::Command::Logout, commandHandler.getCommandForUser("test_logout", testUser));
        EXPECT_EQ(model::Command::Look, commandHandler.getCommandForUser("test_look", testUser));
        EXPECT_EQ(model::Command::Move, commandHandler.getCommandForUser("test_move", testUser));
        EXPECT_EQ(model::Command::Quit, commandHandler.getCommandForUser("test_quit", testUser));
        EXPECT_EQ(model::Command::Register, commandHandler.getCommandForUser("test_register", testUser));
        EXPECT_EQ(model::Command::Say, commandHandler.getCommandForUser("test_say", testUser));
        EXPECT_EQ(model::Command::Shutdown, commandHandler.getCommandForUser("test_shutdown", testUser));
        EXPECT_EQ(model::Command::Tell, commandHandler.getCommandForUser("test_tell", testUser));
        EXPECT_EQ(model::Command::Yell, commandHandler.getCommandForUser("test_yell", testUser));
    }

    TEST(CommandHandlerTestSuite, aliasedCommandsFallBackToDefault) {
        model::CommandHandler commandHandler;
        std::string testUser = "testuser_no_aliases";
        EXPECT_EQ(model::Command::Help, commandHandler.getCommandForUser("help", testUser));
        EXPECT_EQ(model::Command::Login, commandHandler.getCommandForUser("login", testUser));
        EXPECT_EQ(model::Command::Logout, commandHandler.getCommandForUser("logout", testUser));
        EXPECT_EQ(model::Command::Look, commandHandler.getCommandForUser("look", testUser));
        EXPECT_EQ(model::Command::Move, commandHandler.getCommandForUser("move", testUser));
        EXPECT_EQ(model::Command::Quit, commandHandler.getCommandForUser("quit", testUser));
        EXPECT_EQ(model::Command::Register, commandHandler.getCommandForUser("register", testUser));
        EXPECT_EQ(model::Command::Say, commandHandler.getCommandForUser("say", testUser));
        EXPECT_EQ(model::Command::Shutdown, commandHandler.getCommandForUser("shutdown", testUser));
        EXPECT_EQ(model::Command::Tell, commandHandler.getCommandForUser("tell", testUser));
        EXPECT_EQ(model::Command::Yell, commandHandler.getCommandForUser("yell", testUser));
    }
}
