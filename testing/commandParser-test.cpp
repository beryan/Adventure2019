//
// Created by Stephen Wanhella on 2019-03-12.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "CommandParser.h"

using game::Command;
using game::CommandParser;

namespace {
    const auto DEBUG = "debug";
    const auto DROP = "drop";
    const auto EQUIPMENT = "equipment";
    const auto EXAMINE = "examine";
    const auto EXITS = "exits";
    const auto HELP = "help";
    const auto INVENTORY = "inventory";
    const auto LOGIN = "login";
    const auto LOGOUT = "logout";
    const auto LOOK = "look";
    const auto MOVE = "move";
    const auto QUIT = "quit";
    const auto REGISTER = "register";
    const auto REMOVE = "remove";
    const auto SAY = "say";
    const auto SHUTDOWN = "shutdown";
    const auto TALK = "talk";
    const auto TAKE = "take";
    const auto TELL = "tell";
    const auto WEAR = "wear";
    const auto YELL = "yell";
    const auto ALIAS = "alias";

    TEST(CommandParserTestSuite, canGetCommandsFromStrings) {
        CommandParser commandParser;
        EXPECT_EQ(Command::Debug, commandParser.parseCommand(DEBUG));
        EXPECT_EQ(Command::Drop, commandParser.parseCommand(DROP));
        EXPECT_EQ(Command::Equipment, commandParser.parseCommand(EQUIPMENT));
        EXPECT_EQ(Command::Examine, commandParser.parseCommand(EXAMINE));
        EXPECT_EQ(Command::Exits, commandParser.parseCommand(EXITS));
        EXPECT_EQ(Command::Help, commandParser.parseCommand(HELP));
        EXPECT_EQ(Command::Inventory, commandParser.parseCommand(INVENTORY));
        EXPECT_EQ(Command::Login, commandParser.parseCommand(LOGIN));
        EXPECT_EQ(Command::Logout, commandParser.parseCommand(LOGOUT));
        EXPECT_EQ(Command::Look, commandParser.parseCommand(LOOK));
        EXPECT_EQ(Command::Move, commandParser.parseCommand(MOVE));
        EXPECT_EQ(Command::Quit, commandParser.parseCommand(QUIT));
        EXPECT_EQ(Command::Register, commandParser.parseCommand(REGISTER));
        EXPECT_EQ(Command::Remove, commandParser.parseCommand(REMOVE));
        EXPECT_EQ(Command::Say, commandParser.parseCommand(SAY));
        EXPECT_EQ(Command::Shutdown, commandParser.parseCommand(SHUTDOWN));
        EXPECT_EQ(Command::Talk, commandParser.parseCommand(TALK));
        EXPECT_EQ(Command::Take, commandParser.parseCommand(TAKE));
        EXPECT_EQ(Command::Tell, commandParser.parseCommand(TELL));
        EXPECT_EQ(Command::Wear, commandParser.parseCommand(WEAR));
        EXPECT_EQ(Command::Yell, commandParser.parseCommand(YELL));
        EXPECT_EQ(Command::Alias, commandParser.parseCommand(ALIAS));
    }

    TEST(CommandParserTestSuite, canGetStringsFromCommands) {
        CommandParser commandParser;
        EXPECT_EQ(DEBUG, commandParser.getStringForCommand(Command::Debug));
        EXPECT_EQ(DROP, commandParser.getStringForCommand(Command::Drop));
        EXPECT_EQ(EQUIPMENT, commandParser.getStringForCommand(Command::Equipment));
        EXPECT_EQ(EXAMINE, commandParser.getStringForCommand(Command::Examine));
        EXPECT_EQ(EXITS, commandParser.getStringForCommand(Command::Exits));
        EXPECT_EQ(HELP, commandParser.getStringForCommand(Command::Help));
        EXPECT_EQ(INVENTORY, commandParser.getStringForCommand(Command::Inventory));
        EXPECT_EQ(LOGIN, commandParser.getStringForCommand(Command::Login));
        EXPECT_EQ(LOGOUT, commandParser.getStringForCommand(Command::Logout));
        EXPECT_EQ(LOOK, commandParser.getStringForCommand(Command::Look));
        EXPECT_EQ(MOVE, commandParser.getStringForCommand(Command::Move));
        EXPECT_EQ(QUIT, commandParser.getStringForCommand(Command::Quit));
        EXPECT_EQ(REGISTER, commandParser.getStringForCommand(Command::Register));
        EXPECT_EQ(REMOVE, commandParser.getStringForCommand(Command::Remove));
        EXPECT_EQ(SAY, commandParser.getStringForCommand(Command::Say));
        EXPECT_EQ(SHUTDOWN, commandParser.getStringForCommand(Command::Shutdown));
        EXPECT_EQ(TALK, commandParser.getStringForCommand(Command::Talk));
        EXPECT_EQ(TAKE, commandParser.getStringForCommand(Command::Take));
        EXPECT_EQ(TELL, commandParser.getStringForCommand(Command::Tell));
        EXPECT_EQ(WEAR, commandParser.getStringForCommand(Command::Wear));
        EXPECT_EQ(YELL, commandParser.getStringForCommand(Command::Yell));
        EXPECT_EQ(ALIAS, commandParser.getStringForCommand(Command::Alias));
    }

    TEST(CommandParserTestSuite, canGetInvalidCommand) {
        CommandParser commandParser;
        EXPECT_EQ(Command::InvalidCommand, commandParser.parseCommand("asdfasdfasdf"));
    }
}
