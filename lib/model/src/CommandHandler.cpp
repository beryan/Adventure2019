//
// Created by Stephen Wanhella on 2019-02-14.
//

#include <CommandHandler.h>

#include "CommandHandler.h"
#include "Command.h"
#include "json.hpp"

using json = nlohmann::json;
using model::Command;
using model::CommandHandler;

Command CommandHandler::getCommand(const std::string &commandStr) {
    return Command::INVALID_COMMAND;
}

Command CommandHandler::getCommand(const std::string &commandStr, const std::string &username) {
    // todo: use file api

    // check if user has aliased commands

    return Command::INVALID_COMMAND;
}

Command convert(const std::string str) {
    if (str == "HELP") return Command::HELP;
    if (str == "LOGIN") return Command::LOGIN;
    if (str == "LOGOUT") return Command::LOGOUT;
    if (str == "LOOK") return Command::LOOK;
    if (str == "MOVE") return Command::MOVE;
    if (str == "QUIT") return Command::QUIT;
    if (str == "REGISTER") return Command::REGISTER;
    if (str == "SAY") return Command::SAY;
    if (str == "SHUTDOWN") return Command::SHUTDOWN;
    if (str == "TELL") return Command::TELL;
    if (str == "YELL") return Command::YELL;
    return Command::INVALID_COMMAND;
}