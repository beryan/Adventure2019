//
// Created by Stephen Wanhella on 2019-02-14.
//

#include <CommandHandler.h>

#include "CommandHandler.h"
#include "Command.h"
#include "json.hpp"
#include <boost/bimap.hpp>

using json = nlohmann::json;
using model::Command;
using model::CommandHandler;

CommandHandler::CommandHandler() {
    this->defaultCommands.insert(bm_type::value_type("help", Command::Help));
}

Command CommandHandler::getCommand(const std::string &commandStr) {
    return Command::InvalidCommand;
}

Command CommandHandler::getCommand(const std::string &commandStr, const std::string &username) {
    // todo: use file api

    // check if user has aliased commands

    return Command::InvalidCommand;
}

Command convert(const std::string str) {
    if (str == "HELP") return Command::Help;
    if (str == "LOGIN") return Command::Login;
    if (str == "LOGOUT") return Command::Logout;
    if (str == "LOOK") return Command::Look;
    if (str == "MOVE") return Command::Move;
    if (str == "QUIT") return Command::Quit;
    if (str == "REGISTER") return Command::Register;
    if (str == "SAY") return Command::Say;
    if (str == "SHUTDOWN") return Command::Shutdown;
    if (str == "TELL") return Command::Tell;
    if (str == "YELL") return Command::Yell;
    return Command::InvalidCommand;
}