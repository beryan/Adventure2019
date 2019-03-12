//
// Created by Stephen Wanhella on 2019-02-14.
//

#include "AliasManager.h"
#include "DataManager.h"
#include "CommandParser.h"

#include <fstream>

static constexpr auto COMMANDS_FILE_PATH = "lib/data/commands.json";
static constexpr auto GLOBAL_ALIASES_USER = "global_aliases";

using nlohmann::json;
using game::Command;
using game::AliasManager;
using DataManager::writeJson;
using game::CommandParser;

bool AliasManager::findAliasedCommand(const std::string &commandStr, const std::string &username,
                                      Command &result) {
    // todo: use file access abstraction layer
    std::ifstream ifs(COMMANDS_FILE_PATH);

    if (!ifs.is_open()) {
        throw std::runtime_error("Could not open commands file");
    }

    json commands_json = json::parse(ifs);

    bool wasFound = false;
    auto username_iterator = commands_json.find(username);
    if (username_iterator != commands_json.end()) {
        auto aliasedCommands = username_iterator->get<std::unordered_map<std::string, std::string>>();
        auto command_iterator = aliasedCommands.find(commandStr);
        if (command_iterator != aliasedCommands.end()) {
            CommandParser commandParser;
            std::string command = command_iterator->second;
            Command parsedCommand = commandParser.parseCommand(command);
            if (parsedCommand != Command::InvalidCommand) {
                result = parsedCommand;
                wasFound = true;
            }
        }
    }

    return wasFound;
}

void AliasManager::setGlobalAlias(const Command &command, const std::string &alias) {
    setUserAlias(command, alias, GLOBAL_ALIASES_USER);
}

void AliasManager::clearGlobalAlias(const Command &command) {
    clearUserAlias(command, GLOBAL_ALIASES_USER);
}

void AliasManager::setUserAlias(const Command &command, const std::string &alias, const std::string &username) {
    std::ifstream inFile(COMMANDS_FILE_PATH);
    CommandParser commandParser;

    if (!inFile.is_open()) {
        throw std::runtime_error("Could not open commands file");
    }

    json commands_json = json::parse(inFile);

    auto username_iterator = commands_json.find(username);
    if (username_iterator != commands_json.end()) {
        std::string commandStr = commandParser.getStringForCommand(command);
        json newAlias = {{alias, commandStr}};
        username_iterator->update(newAlias);
    } else {
        std::string commandStr = commandParser.getStringForCommand(command);
        commands_json[username] = {{alias, commandStr}};
    }

    inFile.close();

    writeJson(commands_json, COMMANDS_FILE_PATH);
}

void AliasManager::clearUserAlias(const Command &command, const std::string &username) {
    std::ifstream inFile(COMMANDS_FILE_PATH);
    CommandParser commandParser;

    if (!inFile.is_open()) {
        throw std::runtime_error("Could not open commands file");
    }

    json commands_json = json::parse(inFile);

    auto username_iterator = commands_json.find(username);
    if (username_iterator != commands_json.end()) {
        std::string commandStr = commandParser.getStringForCommand(command);
        auto user_aliases = username_iterator->get<std::unordered_map<std::string, std::string>>();
        auto it = std::find_if(user_aliases.begin(), user_aliases.end(), [commandStr](auto alias) {
            return alias.second == commandStr;
        });
        if (it != user_aliases.end()) {
            user_aliases.erase(it);
            json j(user_aliases);
            *username_iterator = j;
        }

        if (username_iterator->empty()) {
            commands_json.erase(username_iterator);
        }
    }

    inFile.close();

    writeJson(commands_json, COMMANDS_FILE_PATH);
}

Command AliasManager::getCommandForUser(const std::string &commandStr, const std::string &username) {
    Command result;
    if (!findAliasedCommand(commandStr, username, result) &&
        !findAliasedCommand(commandStr, GLOBAL_ALIASES_USER, result)) {
        CommandParser commandParser;
        result = commandParser.parseCommand(commandStr);
    }
    return result;
}
