//
// Created by Stephen Wanhella on 2019-02-14.
//

#ifndef COMMAND_H
#define COMMAND_H
namespace model {
    enum class Command {
        Debug,
        Exits,
        Help,
        Info,
        Login,
        Logout,
        Look,
        Move,
        Quit,
        Register,
        Say,
        Shutdown,
        Tell,
        Yell,
        Cast,
        Spells,
        InvalidCommand
    };
}

#endif //COMMAND_H
