//
// Created by Stephen Wanhella on 2019-02-14.
//

#ifndef ALIASMANAGER_H
#define ALIASMANAGER_H

#include "Command.h"
#include "Player.h"
#include <string>
#include <unordered_map>

using game::Command;

static constexpr auto COMMANDS_FILE_PATH = "lib/data/commands.json";

namespace game {
    /**
     * A class to handle the setting, clearing, and listing of aliases
     */
    class AliasManager {
    public:
        AliasManager() : filePath(COMMANDS_FILE_PATH) {};

        explicit AliasManager(std::string_view filePath) : filePath(filePath) {};

        /**
         * Sets an alias for a command that all users can use
         * @param command command to alias
         * @param alias alias for the command
         */
        void setGlobalAlias(const Command &command, std::string_view alias);

        /**
         * Deletes a global alias for a command
         * @param command command to delete the alias for
         */
        void clearGlobalAlias(const Command &command);

        /**
         * Checks if an alias is valid, i.e. doesn't match a default command.
         * e.g. and alias "say" or "yell" is invalid.
         * @param alias
         * @return true if valid, false otherwise
         */
        bool isValidAlias(const std::string &alias);

        /**
         * Sets an alias for a command for a particular user
         * @param command command to alias
         * @param alias alias for the command
         * @param username user to set the alias for
         */
        bool setUserAlias(const Command &command, std::string_view alias, std::string_view username);

        /**
         * Deletes an alias for a user
         * @param command command to clear the alias for
         * @param username user to clear the alias for
         */
        void clearUserAlias(const Command &command, std::string_view username);

        /**
         * Get the appropriate command for a user from string, checking first if user has aliased a command
         * @param commandStr command string to parse
         * @param username username to check for command alias
         * @return Command from commandStr, InvalidCommand if no matching command
         */
        Command getCommandForUser(std::string_view commandStr, std::string_view username);

        /**
         * Get a map of aliases for a user
         * @param username user to get aliases for
         * @return map of aliases
         */
        std::unordered_map<std::string, std::string> getAliasesForUser(std::string_view username);

        /**
         * Get a map of global aliases
         * @return map of global aliases
         */
        std::unordered_map<std::string, std::string> getGlobalAliases();


    private:
        std::string filePath;

        /**
         * Checks for an aliased command for user {username} and sets it to the result.
         * Command json file should be formatted as follows:
         *    {
         *     "[username]": {
         *         "[command alias]": "[command that was aliased]"
         *      }
         *    }
         * @param commandStr command to look for
         * @param username username to find aliased command for
         * @param result command that was aliased. Only gets set if command was found.
         * @return true if command was found, false otherwise
         */
        bool findAliasedCommand(std::string_view commandStr, std::string_view username, Command &result);

        /**
         * transform a string to lowercase
         * @param str string to transform
         */
        std::string toLower(std::string str);
    };
}


#endif //ALIASMANAGER_H
