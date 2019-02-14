//
// Created by Stephen Wanhella on 2019-02-14.
//

#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include "Command.h"
#include "Player.h"
#include <string>

namespace model {
    class CommandHandler {
    public:
        CommandHandler() = default;

        model::Command getCommand(const model::Player &player, const std::string &commandStr);
    };
}


#endif //COMMANDHANDLER_H
