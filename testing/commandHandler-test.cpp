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

        std::string testHelp = "test_help";
        std::string testLogin = "test_login";
        std::string testLogout = "test_logout";
        std::string testLook = "test_look";
        std::string testMove = "test_move";
        std::string testQuit = "test_quit";
        std::string registerAlias = "test_register";
        std::string sayAlias = "test_say";
        std::string shutdownAlias = "test_shutdown";
        std::string tellAlias = "test_tell";
        std::string yellAlias = "test_yell";

        commandHandler.setUserAlias(Command::Help, testHelp, testUser);
        commandHandler.setUserAlias(Command::Login, testLogin, testUser);
        commandHandler.setUserAlias(Command::Logout, testLogout, testUser);
        commandHandler.setUserAlias(Command::Look, testLook, testUser);
        commandHandler.setUserAlias(Command::Move, testMove, testUser);
        commandHandler.setUserAlias(Command::Quit, testQuit, testUser);
        commandHandler.setUserAlias(Command::Register, registerAlias, testUser);
        commandHandler.setUserAlias(Command::Say, sayAlias, testUser);
        commandHandler.setUserAlias(Command::Shutdown, shutdownAlias, testUser);
        commandHandler.setUserAlias(Command::Tell, tellAlias, testUser);
        commandHandler.setUserAlias(Command::Yell, yellAlias, testUser);


        EXPECT_EQ(Command::Help, commandHandler.getCommandForUser(testHelp, testUser));
        EXPECT_EQ(Command::Login, commandHandler.getCommandForUser(testLogin, testUser));
        EXPECT_EQ(Command::Logout, commandHandler.getCommandForUser(testLogout, testUser));
        EXPECT_EQ(Command::Look, commandHandler.getCommandForUser(testLook, testUser));
        EXPECT_EQ(Command::Move, commandHandler.getCommandForUser(testMove, testUser));
        EXPECT_EQ(Command::Quit, commandHandler.getCommandForUser(testQuit, testUser));
        EXPECT_EQ(Command::Register, commandHandler.getCommandForUser(registerAlias, testUser));
        EXPECT_EQ(Command::Say, commandHandler.getCommandForUser(sayAlias, testUser));
        EXPECT_EQ(Command::Shutdown, commandHandler.getCommandForUser(shutdownAlias, testUser));
        EXPECT_EQ(Command::Tell, commandHandler.getCommandForUser(tellAlias, testUser));
        EXPECT_EQ(Command::Yell, commandHandler.getCommandForUser(yellAlias, testUser));

        commandHandler.clearUserAlias(Command::Help, testUser);
        commandHandler.clearUserAlias(Command::Login, testUser);
        commandHandler.clearUserAlias(Command::Logout, testUser);
        commandHandler.clearUserAlias(Command::Look, testUser);
        commandHandler.clearUserAlias(Command::Move, testUser);
        commandHandler.clearUserAlias(Command::Quit, testUser);
        commandHandler.clearUserAlias(Command::Register, testUser);
        commandHandler.clearUserAlias(Command::Say, testUser);
        commandHandler.clearUserAlias(Command::Shutdown, testUser);
        commandHandler.clearUserAlias(Command::Tell, testUser);
        commandHandler.clearUserAlias(Command::Yell, testUser);

        EXPECT_EQ(Command::InvalidCommand, commandHandler.getCommandForUser(testHelp, testUser));
        EXPECT_EQ(Command::InvalidCommand, commandHandler.getCommandForUser(testLogin, testUser));
        EXPECT_EQ(Command::InvalidCommand, commandHandler.getCommandForUser(testLogout, testUser));
        EXPECT_EQ(Command::InvalidCommand, commandHandler.getCommandForUser(testLook, testUser));
        EXPECT_EQ(Command::InvalidCommand, commandHandler.getCommandForUser(testMove, testUser));
        EXPECT_EQ(Command::InvalidCommand, commandHandler.getCommandForUser(testQuit, testUser));
        EXPECT_EQ(Command::InvalidCommand, commandHandler.getCommandForUser(registerAlias, testUser));
        EXPECT_EQ(Command::InvalidCommand, commandHandler.getCommandForUser(sayAlias, testUser));
        EXPECT_EQ(Command::InvalidCommand, commandHandler.getCommandForUser(shutdownAlias, testUser));
        EXPECT_EQ(Command::InvalidCommand, commandHandler.getCommandForUser(tellAlias, testUser));
        EXPECT_EQ(Command::InvalidCommand, commandHandler.getCommandForUser(yellAlias, testUser));
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
        std::string helpAlias = "test_help";
        std::string helpDefault = commandHandler.getStringForCommand(Command::Help);

        commandHandler.setGlobalAlias(Command::Help, global_help_alias);
        commandHandler.setUserAlias(Command::Help, helpAlias, testUser);

        EXPECT_EQ(Command::Help, commandHandler.getCommandForUser(helpDefault, testUser));
        EXPECT_EQ(Command::Help, commandHandler.getCommandForUser(helpAlias, testUser));
        EXPECT_EQ(Command::Help, commandHandler.getCommandForUser(global_help_alias, testUser));

        EXPECT_EQ(Command::Help, commandHandler.getCommandForUser(helpDefault, testUser_noAliases));
        EXPECT_EQ(Command::InvalidCommand, commandHandler.getCommandForUser(helpAlias, testUser_noAliases));
        EXPECT_EQ(Command::Help, commandHandler.getCommandForUser(global_help_alias, testUser_noAliases));
        commandHandler.clearGlobalAlias(Command::Help);
        commandHandler.clearUserAlias(Command::Help, testUser);
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
