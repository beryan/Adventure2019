//
// Created by Stephen Wanhella on 2019-03-12.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "CommandParser.h"

using game::Command;
using game::CommandParser;

namespace {
    const auto ALIAS = "alias";
    const auto CAST = "cast";
    const auto CHAT = "chat";
    const auto BUILD = "build";
    const auto DROP = "drop";
    const auto EQUIPMENT = "equipment";
    const auto EXAMINE = "examine";
    const auto EXITS = "exits";
    const auto GIVE = "give";
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
    const auto SPELLS = "spells";
    const auto TAKE = "take";
    const auto TALK = "talk";
    const auto TELL = "tell";
    const auto WEAR = "wear";
    const auto YELL = "yell";

    const auto ACREATE = "acreate";
    const auto RCREATE = "rcreate";
    const auto OCREATE = "ocreate";
    const auto NCREATE = "ncreate";
    const auto AEDIT = "aedit";
    const auto REDIT = "redit";
    const auto OEDIT = "oedit";
    const auto NEDIT = "nedit";
    const auto ORESET = "oreset";
    const auto NRESET = "nreset";
    const auto ALIST = "alist";
    const auto RLIST = "rlist";
    const auto OLIST = "olist";
    const auto NLIST = "nlist";
    const auto GOTO = "goto";
    const auto RESET = "reset";

    TEST(CommandParserTestSuite, canGetCommandsFromStrings) {
        CommandParser commandParser;
        EXPECT_EQ(Command::Alias, commandParser.parseCommand(ALIAS));
        EXPECT_EQ(Command::Cast, commandParser.parseCommand(CAST));
        EXPECT_EQ(Command::Chat, commandParser.parseCommand(CHAT));
        EXPECT_EQ(Command::Build, commandParser.parseCommand(BUILD));
        EXPECT_EQ(Command::Drop, commandParser.parseCommand(DROP));
        EXPECT_EQ(Command::Equipment, commandParser.parseCommand(EQUIPMENT));
        EXPECT_EQ(Command::Examine, commandParser.parseCommand(EXAMINE));
        EXPECT_EQ(Command::Exits, commandParser.parseCommand(EXITS));
        EXPECT_EQ(Command::Give, commandParser.parseCommand(GIVE));
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
        EXPECT_EQ(Command::Spells, commandParser.parseCommand(SPELLS));
        EXPECT_EQ(Command::Take, commandParser.parseCommand(TAKE));
        EXPECT_EQ(Command::Talk, commandParser.parseCommand(TALK));
        EXPECT_EQ(Command::Tell, commandParser.parseCommand(TELL));
        EXPECT_EQ(Command::Wear, commandParser.parseCommand(WEAR));
        EXPECT_EQ(Command::Yell, commandParser.parseCommand(YELL));

        EXPECT_EQ(Command::Acreate, commandParser.parseCommand(ACREATE));
        EXPECT_EQ(Command::Rcreate, commandParser.parseCommand(RCREATE));
        EXPECT_EQ(Command::Ocreate, commandParser.parseCommand(OCREATE));
        EXPECT_EQ(Command::Ncreate, commandParser.parseCommand(NCREATE));
        EXPECT_EQ(Command::Aedit, commandParser.parseCommand(AEDIT));
        EXPECT_EQ(Command::Redit, commandParser.parseCommand(REDIT));
        EXPECT_EQ(Command::Oedit, commandParser.parseCommand(OEDIT));
        EXPECT_EQ(Command::Nedit, commandParser.parseCommand(NEDIT));
        EXPECT_EQ(Command::Oreset, commandParser.parseCommand(ORESET));
        EXPECT_EQ(Command::Nreset, commandParser.parseCommand(NRESET));
        EXPECT_EQ(Command::Alist, commandParser.parseCommand(ALIST));
        EXPECT_EQ(Command::Rlist, commandParser.parseCommand(RLIST));
        EXPECT_EQ(Command::Olist, commandParser.parseCommand(OLIST));
        EXPECT_EQ(Command::Nlist, commandParser.parseCommand(NLIST));
        EXPECT_EQ(Command::Goto, commandParser.parseCommand(GOTO));
        EXPECT_EQ(Command::Reset, commandParser.parseCommand(RESET));
    }

    TEST(CommandParserTestSuite, canGetStringsFromCommands) {
        CommandParser commandParser;
        EXPECT_EQ(ALIAS, commandParser.getStringForCommand(Command::Alias));
        EXPECT_EQ(CAST, commandParser.getStringForCommand(Command::Cast));
        EXPECT_EQ(CHAT, commandParser.getStringForCommand(Command::Chat));
        EXPECT_EQ(BUILD, commandParser.getStringForCommand(Command::Build));
        EXPECT_EQ(DROP, commandParser.getStringForCommand(Command::Drop));
        EXPECT_EQ(EQUIPMENT, commandParser.getStringForCommand(Command::Equipment));
        EXPECT_EQ(EXAMINE, commandParser.getStringForCommand(Command::Examine));
        EXPECT_EQ(EXITS, commandParser.getStringForCommand(Command::Exits));
        EXPECT_EQ(GIVE, commandParser.getStringForCommand(Command::Give));
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
        EXPECT_EQ(SPELLS, commandParser.getStringForCommand(Command::Spells));
        EXPECT_EQ(TAKE, commandParser.getStringForCommand(Command::Take));
        EXPECT_EQ(TALK, commandParser.getStringForCommand(Command::Talk));
        EXPECT_EQ(TELL, commandParser.getStringForCommand(Command::Tell));
        EXPECT_EQ(WEAR, commandParser.getStringForCommand(Command::Wear));
        EXPECT_EQ(YELL, commandParser.getStringForCommand(Command::Yell));

        EXPECT_EQ(ACREATE, commandParser.getStringForCommand(Command::Acreate));
        EXPECT_EQ(RCREATE, commandParser.getStringForCommand(Command::Rcreate));
        EXPECT_EQ(OCREATE, commandParser.getStringForCommand(Command::Ocreate));
        EXPECT_EQ(NCREATE, commandParser.getStringForCommand(Command::Ncreate));
        EXPECT_EQ(AEDIT, commandParser.getStringForCommand(Command::Aedit));
        EXPECT_EQ(REDIT, commandParser.getStringForCommand(Command::Redit));
        EXPECT_EQ(OEDIT, commandParser.getStringForCommand(Command::Oedit));
        EXPECT_EQ(NEDIT, commandParser.getStringForCommand(Command::Nedit));
        EXPECT_EQ(ORESET, commandParser.getStringForCommand(Command::Oreset));
        EXPECT_EQ(NRESET, commandParser.getStringForCommand(Command::Nreset));
        EXPECT_EQ(ALIST, commandParser.getStringForCommand(Command::Alist));
        EXPECT_EQ(RLIST, commandParser.getStringForCommand(Command::Rlist));
        EXPECT_EQ(OLIST, commandParser.getStringForCommand(Command::Olist));
        EXPECT_EQ(NLIST, commandParser.getStringForCommand(Command::Nlist));
        EXPECT_EQ(GOTO, commandParser.getStringForCommand(Command::Goto));
        EXPECT_EQ(RESET, commandParser.getStringForCommand(Command::Reset));
    }

    TEST(CommandParserTestSuite, canGetInvalidCommand) {
        CommandParser commandParser;
        EXPECT_EQ(Command::InvalidCommand, commandParser.parseCommand("asdfasdfasdf"));
    }
}
