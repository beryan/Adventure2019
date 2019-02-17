//
// Created by Stephen Wanhella on 2019-02-14.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "CommandHandler.h"

namespace {
    TEST(CommandHandlerTestSuite, canGetCommandsFromStrings) {
        model::CommandHandler commandHandler;
        EXPECT_EQ(model::Command::Help, commandHandler.getCommand("help"));
        EXPECT_EQ(model::Command::Login, commandHandler.getCommand("login"));
        EXPECT_EQ(model::Command::Logout, commandHandler.getCommand("logout"));
        EXPECT_EQ(model::Command::Look, commandHandler.getCommand("look"));
        EXPECT_EQ(model::Command::Move, commandHandler.getCommand("move"));
        EXPECT_EQ(model::Command::Quit, commandHandler.getCommand("quit"));
        EXPECT_EQ(model::Command::Register, commandHandler.getCommand("register"));
        EXPECT_EQ(model::Command::Say, commandHandler.getCommand("say"));
        EXPECT_EQ(model::Command::Shutdown, commandHandler.getCommand("shutdown"));
        EXPECT_EQ(model::Command::Tell, commandHandler.getCommand("tell"));
        EXPECT_EQ(model::Command::Yell, commandHandler.getCommand("yell"));
    }

    TEST(CommandHandlerTestSuite, canGetInvalidCommand) {
        model::CommandHandler commandHandler;
        EXPECT_EQ(model::Command::InvalidCommand, commandHandler.getCommand("asdfasdfasdf"));
    }

    TEST(CommandHandlerTestSuite, canGetAliasedCommands) {
        model::CommandHandler commandHandler;
        std::string testUser = "testuser";
        EXPECT_EQ(model::Command::Help, commandHandler.getCommand("test_help", testUser));
        EXPECT_EQ(model::Command::Login, commandHandler.getCommand("test_login", testUser));
        EXPECT_EQ(model::Command::Logout, commandHandler.getCommand("test_logout", testUser));
        EXPECT_EQ(model::Command::Look, commandHandler.getCommand("test_look", testUser));
        EXPECT_EQ(model::Command::Move, commandHandler.getCommand("test_move", testUser));
        EXPECT_EQ(model::Command::Quit, commandHandler.getCommand("test_quit", testUser));
        EXPECT_EQ(model::Command::Register, commandHandler.getCommand("test_register", testUser));
        EXPECT_EQ(model::Command::Say, commandHandler.getCommand("test_say", testUser));
        EXPECT_EQ(model::Command::Shutdown, commandHandler.getCommand("test_shutdown", testUser));
        EXPECT_EQ(model::Command::Tell, commandHandler.getCommand("test_tell", testUser));
        EXPECT_EQ(model::Command::Yell, commandHandler.getCommand("test_yell", testUser));
    }

    TEST(CommandHandlerTestSuite, aliasedCommandsFallBackToDefault) {
        model::CommandHandler commandHandler;
        std::string testUser = "testuser_no_aliases";
        EXPECT_EQ(model::Command::Help, commandHandler.getCommand("help", testUser));
        EXPECT_EQ(model::Command::Login, commandHandler.getCommand("login", testUser));
        EXPECT_EQ(model::Command::Logout, commandHandler.getCommand("logout", testUser));
        EXPECT_EQ(model::Command::Look, commandHandler.getCommand("look", testUser));
        EXPECT_EQ(model::Command::Move, commandHandler.getCommand("move", testUser));
        EXPECT_EQ(model::Command::Quit, commandHandler.getCommand("quit", testUser));
        EXPECT_EQ(model::Command::Register, commandHandler.getCommand("register", testUser));
        EXPECT_EQ(model::Command::Say, commandHandler.getCommand("say", testUser));
        EXPECT_EQ(model::Command::Shutdown, commandHandler.getCommand("shutdown", testUser));
        EXPECT_EQ(model::Command::Tell, commandHandler.getCommand("tell", testUser));
        EXPECT_EQ(model::Command::Yell, commandHandler.getCommand("yell", testUser));
    }
}
