//
// Created by Stephen Wanhella on 2019-02-14.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "CommandHandler.h"

using model::Command;
using model::CommandHandler;

namespace {
    TEST(CommandHandlerTestSuite, canGetCommandsFromStrings) {
        CommandHandler commandHandler;
        EXPECT_EQ(Command::Help, commandHandler.getDefaultCommand("help"));
        EXPECT_EQ(Command::Login, commandHandler.getDefaultCommand("login"));
        EXPECT_EQ(Command::Logout, commandHandler.getDefaultCommand("logout"));
        EXPECT_EQ(Command::Look, commandHandler.getDefaultCommand("look"));
        EXPECT_EQ(Command::Move, commandHandler.getDefaultCommand("move"));
        EXPECT_EQ(Command::Quit, commandHandler.getDefaultCommand("quit"));
        EXPECT_EQ(Command::Register, commandHandler.getDefaultCommand("register"));
        EXPECT_EQ(Command::Say, commandHandler.getDefaultCommand("say"));
        EXPECT_EQ(Command::Shutdown, commandHandler.getDefaultCommand("shutdown"));
        EXPECT_EQ(Command::Tell, commandHandler.getDefaultCommand("tell"));
        EXPECT_EQ(Command::Yell, commandHandler.getDefaultCommand("yell"));
    }

    TEST(CommandHandlerTestSuite, canGetInvalidCommand) {
        CommandHandler commandHandler;
        EXPECT_EQ(Command::InvalidCommand, commandHandler.getDefaultCommand("asdfasdfasdf"));
    }

    TEST(CommandHandlerTestSuite, canGetAliasedCommands) {
        CommandHandler commandHandler;
        std::string testUser = "testuser";
        EXPECT_EQ(Command::Help, commandHandler.getCommandForUser("test_help", testUser));
        EXPECT_EQ(Command::Login, commandHandler.getCommandForUser("test_login", testUser));
        EXPECT_EQ(Command::Logout, commandHandler.getCommandForUser("test_logout", testUser));
        EXPECT_EQ(Command::Look, commandHandler.getCommandForUser("test_look", testUser));
        EXPECT_EQ(Command::Move, commandHandler.getCommandForUser("test_move", testUser));
        EXPECT_EQ(Command::Quit, commandHandler.getCommandForUser("test_quit", testUser));
        EXPECT_EQ(Command::Register, commandHandler.getCommandForUser("test_register", testUser));
        EXPECT_EQ(Command::Say, commandHandler.getCommandForUser("test_say", testUser));
        EXPECT_EQ(Command::Shutdown, commandHandler.getCommandForUser("test_shutdown", testUser));
        EXPECT_EQ(Command::Tell, commandHandler.getCommandForUser("test_tell", testUser));
        EXPECT_EQ(Command::Yell, commandHandler.getCommandForUser("test_yell", testUser));
    }

    TEST(CommandHandlerTestSuite, aliasedCommandsFallBackToDefault) {
        CommandHandler commandHandler;
        std::string testUser = "testuser_no_aliases";
        EXPECT_EQ(Command::Help, commandHandler.getCommandForUser("help", testUser));
        EXPECT_EQ(Command::Login, commandHandler.getCommandForUser("login", testUser));
        EXPECT_EQ(Command::Logout, commandHandler.getCommandForUser("logout", testUser));
        EXPECT_EQ(Command::Look, commandHandler.getCommandForUser("look", testUser));
        EXPECT_EQ(Command::Move, commandHandler.getCommandForUser("move", testUser));
        EXPECT_EQ(Command::Quit, commandHandler.getCommandForUser("quit", testUser));
        EXPECT_EQ(Command::Register, commandHandler.getCommandForUser("register", testUser));
        EXPECT_EQ(Command::Say, commandHandler.getCommandForUser("say", testUser));
        EXPECT_EQ(Command::Shutdown, commandHandler.getCommandForUser("shutdown", testUser));
        EXPECT_EQ(Command::Tell, commandHandler.getCommandForUser("tell", testUser));
        EXPECT_EQ(Command::Yell, commandHandler.getCommandForUser("yell", testUser));
    }

    TEST(CommandHandlerTestSuite, canMakeAndDeleteGlobalAliases) {
        CommandHandler commandHandler;
        std::string testUser = "testuser";
        std::string testUser_noAliases = "asdf_user";
        std::string global_help_alias = "hAlP";

        commandHandler.setGlobalAlias(Command::Help, global_help_alias);
        EXPECT_EQ(Command::Help, commandHandler.getCommandForUser("test_help", testUser));
        EXPECT_EQ(Command::Help, commandHandler.getCommandForUser(global_help_alias, testUser_noAliases));
        EXPECT_EQ(Command::Help, commandHandler.getCommandForUser("help", testUser_noAliases));
        commandHandler.clearGlobalAlias(Command::Help);
        EXPECT_EQ(Command::InvalidCommand, commandHandler.getCommandForUser(global_help_alias, testUser_noAliases));
    }

    TEST(CommandHandlerTestSuite, canMakeAndDeleteUserAliases) {
        CommandHandler commandHandler;
        std::string testUser = "new_test_user";
        std::string testUser_noAliases = "asdf_user";
        std::string user_help_alias = "hAlP";

        commandHandler.setUserAlias(Command::Help, user_help_alias, testUser);
        EXPECT_EQ(Command::Help, commandHandler.getCommandForUser(user_help_alias, testUser));
        EXPECT_NE(Command::Help, commandHandler.getCommandForUser(user_help_alias, testUser_noAliases));
        EXPECT_EQ(Command::Help, commandHandler.getCommandForUser("help", testUser));
        commandHandler.clearUserAlias(Command::Help, testUser);
        EXPECT_EQ(Command::InvalidCommand, commandHandler.getCommandForUser(user_help_alias, testUser));
    }
}
