//
// Created by Stephen Wanhella on 2019-03-12.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "CommandParser.h"

using game::Command;
using game::CommandParser;

namespace {
    TEST(CommandParserTestSuite, canGetCommandsFromStrings) {
        CommandParser commandParser;
        EXPECT_EQ(Command::Help, commandParser.parseCommand("help"));
        EXPECT_EQ(Command::Login, commandParser.parseCommand("login"));
        EXPECT_EQ(Command::Logout, commandParser.parseCommand("logout"));
        EXPECT_EQ(Command::Look, commandParser.parseCommand("look"));
        EXPECT_EQ(Command::Move, commandParser.parseCommand("move"));
        EXPECT_EQ(Command::Quit, commandParser.parseCommand("quit"));
        EXPECT_EQ(Command::Register, commandParser.parseCommand("register"));
        EXPECT_EQ(Command::Say, commandParser.parseCommand("say"));
        EXPECT_EQ(Command::Shutdown, commandParser.parseCommand("shutdown"));
        EXPECT_EQ(Command::Tell, commandParser.parseCommand("tell"));
        EXPECT_EQ(Command::Yell, commandParser.parseCommand("yell"));
    }

    TEST(CommandParserTestSuite, canGetInvalidCommand) {
        CommandParser commandParser;
        EXPECT_EQ(Command::InvalidCommand, commandParser.parseCommand("asdfasdfasdf"));
    }
}
