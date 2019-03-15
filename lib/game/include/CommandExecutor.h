//
// Created by Stephen Wanhella on 3/14/2019.
//

#ifndef WEBSOCKETNETWORKING_COMMANDEXECUTOR_H
#define WEBSOCKETNETWORKING_COMMANDEXECUTOR_H

#include <string>
#include <vector>
#include "Command.h"
#include "Server.h"

using networking::Connection;

namespace game {
    class CommandExecutor {
    public:
        /**
         * Executes a command
         * @param command command to execute
         * @param params parameters for the command
         * @return result of the command
         */
        std::string executeCommand(const Connection &client, const Command &command,
                                   const std::vector<std::string> &params);

    private:
        std::string executeAliasCommand(const Connection &client, const std::vector<std::string> &params);

        template<typename T>
        bool
        containsKeyword(const std::vector<T> &objects, const std::string &keyword);

        template<typename T>
        T
        getItemByKeyword(const std::vector<T> &objects, const std::string &keyword);
    };
}

#endif //WEBSOCKETNETWORKING_COMMANDEXECUTOR_H
