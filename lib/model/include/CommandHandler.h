//
// Created by Stephen Wanhella on 2019-02-14.
//

#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include "Command.h"
#include "Player.h"
#include <string>
#include <boost/bimap.hpp>

typedef boost::bimap<std::string,model::Command> bm_type;

namespace model {
    class CommandHandler {
    public:
        CommandHandler();

        /**
         * Gets default command from string
         * @param commandStr command string to parse
         * @return Command for string. INVALID_COMMAND if no matching command
         */
        model::Command getCommand(const std::string &commandStr);

        /**
         * Get command from string, checking first if user has aliased a command
         * @param commandStr command string to parse
         * @param username username to check for command alias
         * @return Command from string, INVALID_COMMAND if no matching command
         */
        model::Command getCommand(const std::string &commandStr, const std::string &username);

    private:
        bm_type defaultCommands;
    };
}


#endif //COMMANDHANDLER_H
