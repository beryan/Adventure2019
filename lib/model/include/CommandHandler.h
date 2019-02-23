//
// Created by Stephen Wanhella on 2019-02-14.
//

#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include "Command.h"
#include "Player.h"
#include <string>
#include <unordered_map>

using model::Command;

namespace model {
    class CommandHandler {
    public:
        /**
         * Gets default command from string
         * @param commandStr command string to parse
         * @return Command that matches commandStr. InvalidCommand if no matching command
         */
        model::Command getDefaultCommand(const std::string &commandStr);

        /**
         * Get the appropriate command for a user from string, checking first if user has aliased a command
         * @param commandStr command string to parse
         * @param username username to check for command alias
         * @return Command from commandStr, InvalidCommand if no matching command
         */
        model::Command getCommandForUser(const std::string &commandStr, const std::string &username);

        /**
         * Sets an alias for a command that all users can use
         * @param command command to alias
         * @param alias alias for the command
         */
        void setGlobalAlias(Command command, const std::string &alias);

        /**
         * Deletes a global alias for a command
         * @param command command to delete the alias for
         */
        void clearGlobalAlias(Command command);

        /**
         * Sets an alias for a command for a particular user
         * @param command command to alias
         * @param alias alias for the command
         * @param username user to set the alias for
         */
        void setUserAlias(Command command, const std::string &alias, const std::string &username);

        /**
         * Deletes an alias for a user
         * @param command command to clear the alias for
         * @param username user to clear the alias for
         */
        void clearUserAlias(Command command, const std::string &username);

    private:
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
        bool findAliasedCommand(const std::string &commandStr, const std::string &username, Command &result);

        std::string getStringFromCommand(Command command);

        std::unordered_map<std::string, model::Command> commands = {
                {"help",     Command::Help},
                {"login",    Command::Login},
                {"logout",   Command::Logout},
                {"look",     Command::Look},
                {"move",     Command::Move},
                {"quit",     Command::Quit},
                {"register", Command::Register},
                {"say",      Command::Say},
                {"shutdown", Command::Shutdown},
                {"tell",     Command::Tell},
                {"yell",     Command::Yell}
        };
    };
}


#endif //COMMANDHANDLER_H
