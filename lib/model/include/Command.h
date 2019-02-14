//
// Created by Stephen Wanhella on 2019-02-14.
//

#ifndef COMMAND_H
#define COMMAND_H
namespace model {
    enum class Command {
        HELP,
        LOGIN,
        LOGOUT,
        LOOK,
        MOVE,
        QUIT,
        REGISTER,
        SAY,
        SHUTDOWN,
        TELL,
        YELL,
        INVALID_COMMAND
    };
}

#endif //COMMAND_H
