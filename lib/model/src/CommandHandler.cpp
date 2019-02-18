//
// Created by Stephen Wanhella on 2019-02-14.
//

#include <fstream>
#include "CommandHandler.h"
#include "Command.h"
#include "json.hpp"

using nlohmann::json;
using model::Command;
using model::CommandHandler;

const std::string COMMANDS_FILE_PATH = "lib/data/commands.json";
const std::string GLOBAL_ALIAS_USER = "global_aliases";

Command CommandHandler::getDefaultCommand(const std::string &commandStr) {
    Command res = Command::InvalidCommand;
    auto it = this->commands.find(commandStr);
    if (it != this->commands.end()) {
        res = it->second;
    }

    return res;
}

Command CommandHandler::getCommandForUser(const std::string &commandStr, const std::string &username) {
    Command result;
    if (!findAliasedCommand(commandStr, username, result) && !findAliasedCommand(commandStr, GLOBAL_ALIAS_USER, result)) {
        result = getDefaultCommand(commandStr);
    }
    return result;
}

bool CommandHandler::findAliasedCommand(const std::string &commandStr, const std::string &username, Command &result) {
    // todo: use file access abstraction layer
    std::ifstream ifs(COMMANDS_FILE_PATH);
    json t = json::parse(ifs);

    bool wasFound = false;
    auto username_iterator = t.find(username);
    if (username_iterator != t.end()) {
        auto aliasedCommands = username_iterator->get<std::unordered_map<std::string, std::string>>();
        auto command_iterator = aliasedCommands.find(commandStr);
        if (command_iterator != aliasedCommands.end()) {
            std::string command = command_iterator->second;
            auto it = this->commands.find(command);
            if (it != this->commands.end()) {
                result = it->second;
                wasFound = true;
            }
        }
    }

    return wasFound;
}