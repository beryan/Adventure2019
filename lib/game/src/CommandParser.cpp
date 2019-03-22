//
// Created by Stephen Wanhella on 2019-03-12.
//

#include <algorithm>
#include <iostream>
#include "CommandParser.h"
#include "AliasManager.h"

using game::Command;
using game::CommandParser;

Command CommandParser::parseCommand(std::string_view commandStr) {
    Command res = Command::InvalidCommand;
    auto it = std::find_if(this->commands.begin(), this->commands.end(),
                           [&commandStr](const auto &command) {
                               return commandStr.compare(command.first) == 0;
                           });
    if (it != this->commands.end()) {
        res = it->second;
    }

    return res;
}

std::string CommandParser::getStringForCommand(const Command &command) {
    std::string res;
    auto it = std::find_if(this->commands.begin(), this->commands.end(),
                           [command](const std::pair<std::string, Command> &pair) {
                               return pair.second == command;
                           });

    if (it != this->commands.end()) {
        res = it->first;
    }

    return res;
}