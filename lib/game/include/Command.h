//
// Created by Stephen Wanhella on 2019-02-14.
//

#ifndef COMMAND_H
#define COMMAND_H
namespace game {
    enum class Command {
        Attack,
        Cast,
        Chat,
        Debug,
        Drop,
        Equipment,
        Examine,
        Exits,
        Give,
        Help,
        Inventory,
        Login,
        Logout,
        Look,
        Move,
        Quit,
        Register,
        Remove,
        Say,
        Shutdown,
        Spells,
        Talk,
        Take,
        Tell,
        Wear,
        Yell,
        Alias,
        InvalidCommand
    };
}

#endif //COMMAND_H
