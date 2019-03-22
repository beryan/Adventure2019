//
// Created by Stephen Wanhella on 2019-02-14.
//

#include "AliasManager.h"
#include "DataManager.h"
#include "CommandParser.h"

#include <fstream>
#include <locale>
#include <boost/algorithm/string/case_conv.hpp>

static constexpr auto GLOBAL_ALIASES_USER = "global_aliases";

using nlohmann::json;
using game::Command;
using game::AliasManager;
using DataManager::writeJson;
using game::CommandParser;

bool AliasManager::findAliasedCommand(std::string_view commandStr, std::string_view username,
                                      Command &result) {
    // todo: use file access abstraction layer
    std::ifstream ifs(this->filePath);

    if (!ifs.is_open()) {
        throw std::runtime_error("Could not open commands file");
    }

    json commands_json = json::parse(ifs);

    bool wasFound = false;
    auto username_iterator = commands_json.find(username);
    if (username_iterator != commands_json.end()) {
        auto aliasedCommands = username_iterator->get<std::unordered_map<std::string, std::string>>();
        auto command_iterator = std::find_if(aliasedCommands.begin(), aliasedCommands.end(),
                                             [commandStr](const auto &aliasedCommand) {
                                                 return aliasedCommand.first == commandStr;
                                             });
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

bool AliasManager::setGlobalAlias(const Command &command, std::string_view alias) {
    return setUserAlias(command, alias, GLOBAL_ALIASES_USER);
}

void AliasManager::clearGlobalAlias(const Command &command) {
    clearUserAlias(command, GLOBAL_ALIASES_USER);
}

bool AliasManager::setUserAlias(const Command &command, std::string_view alias, std::string_view username) {
    bool didSetAlias = true;
    std::ifstream inFile(this->filePath);

    if (!inFile.is_open()) {
        throw std::runtime_error("Could not open commands file");
    }

    CommandParser commandParser;
    json commands_json = json::parse(inFile);
    auto username_iterator = commands_json.find(username);
    if (username_iterator != commands_json.end()) {
        std::string commandStr = commandParser.getStringForCommand(command);
        auto user_aliases = username_iterator->get<std::unordered_map<std::string, std::string>>();
        auto alias_iterator = std::find_if(user_aliases.begin(), user_aliases.end(),
                                           [&alias, &commandStr](const auto &alias_pair) {
                                               return alias_pair.first == alias || alias_pair.second == commandStr;
                                           });
        if (alias_iterator == user_aliases.end()) {
            json newAlias = {{alias, commandStr}};
            username_iterator->update(newAlias);
        } else {
            didSetAlias = false;
        }
    } else {
        std::string commandStr = commandParser.getStringForCommand(command);
        commands_json[std::string(username)] = {{alias, commandStr}};
    }

    inFile.close();

    writeJson(commands_json, this->filePath);
    return didSetAlias;
}

void AliasManager::clearUserAlias(const Command &command, std::string_view username) {
    std::ifstream inFile(this->filePath);
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

    writeJson(commands_json, this->filePath);
}

Command AliasManager::getCommandForUser(std::string_view commandStr, std::string_view username) {
    Command result;
    if (!findAliasedCommand(commandStr, username, result) &&
        !findAliasedCommand(commandStr, GLOBAL_ALIASES_USER, result)) {
        CommandParser commandParser;
        result = commandParser.parseCommand(commandStr);
    }
    return result;
}

std::unordered_map<std::string, std::string> AliasManager::getAliasesForUser(std::string_view username) {
    std::unordered_map<std::string, std::string> aliases;

    std::ifstream ifs(COMMANDS_FILE_PATH);

    if (!ifs.is_open()) {
        throw std::runtime_error("Could not open commands file");
    }

    json commands_json = json::parse(ifs);

    auto username_iterator = commands_json.find(username);
    if (username_iterator != commands_json.end()) {
        aliases = username_iterator->get<std::unordered_map<std::string, std::string>>();

    }

    return aliases;
}

std::unordered_map<std::string, std::string> AliasManager::getGlobalAliases() {
    return getAliasesForUser(GLOBAL_ALIASES_USER);
}

bool AliasManager::isValidAlias(const std::string &alias) {
    CommandParser commandParser;
    std::string lower = boost::algorithm::to_lower_copy(alias);;
    Command command = commandParser.parseCommand(lower);
    return command == Command::InvalidCommand;
}
