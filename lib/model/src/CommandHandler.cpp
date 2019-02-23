//
// Created by Stephen Wanhella on 2019-02-14.
//

#include <fstream>
#include "CommandHandler.h"
#include "Command.h"
#include "json.hpp"
#include <iomanip>
#include <CommandHandler.h>


using nlohmann::json;
using model::Command;
using model::CommandHandler;

const std::string COMMANDS_FILE_PATH = "lib/data/commands.json";
const std::string GLOBAL_ALIAS_USER = "global_aliases";

void writeJson(json j);

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
    if (!findAliasedCommand(commandStr, username, result) &&
        !findAliasedCommand(commandStr, GLOBAL_ALIAS_USER, result)) {
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

void CommandHandler::setGlobalAlias(Command command, const std::string &alias) {
    setUserAlias(command, alias, GLOBAL_ALIAS_USER);
}

void CommandHandler::clearGlobalAlias(Command command) {
    clearUserAlias(command, GLOBAL_ALIAS_USER);
}

void model::CommandHandler::setUserAlias(Command command, const std::string &alias, const std::string &username) {
    std::ifstream inFile(COMMANDS_FILE_PATH);

    json t = json::parse(inFile);

    auto username_iterator = t.find(username);
    if (username_iterator != t.end()) {
        std::string commandStr = getStringFromCommand(command);
        json newAlias = {{alias, commandStr}};
        username_iterator->update(newAlias);
    } else {
        std::string commandStr = getStringFromCommand(command);
        t[username] = {{alias, commandStr}};
    }

    inFile.close();

    writeJson(t);
}

void model::CommandHandler::clearUserAlias(Command command, const std::string &username) {
    std::ifstream inFile(COMMANDS_FILE_PATH);

    json t = json::parse(inFile);

    auto username_iterator = t.find(username);
    if (username_iterator != t.end()) {
        std::string commandStr = getStringFromCommand(command);
        auto m = username_iterator->get<std::unordered_map<std::string, std::string>>();
        auto it = m.begin();
        while (it != m.end() && it->second != commandStr) {
            it++;
        }
        if (it->second == commandStr) {
            m.erase(it);
            json j(m);
            *username_iterator = j;
        }
    }

    inFile.close();

    writeJson(t);
}

std::string CommandHandler::getStringFromCommand(Command command) {
    std::string res;
    for (const auto &kv : this->commands) {
        if (kv.second == command) {
            res = kv.first;
        }
    }

    return res;
}

void writeJson(json j) {
    std::ofstream outFile(COMMANDS_FILE_PATH);
    outFile << std::setw(2) << j;
}
