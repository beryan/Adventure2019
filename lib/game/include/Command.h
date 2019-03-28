//
// Created by Stephen Wanhella on 2019-02-14.
//

#ifndef COMMAND_H
#define COMMAND_H
namespace game {
    enum class Command {
        Attack,
        Build,
        Cast,
        Chat,
        Drop,
        Equipment,
        Examine,
        Exits,
        Flee,
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
        Status,
        Talk,
        Take,
        Tell,
        Wear,
        Yell,
        Alias,

        Acreate,
        Rcreate,
        Ocreate,
        Ncreate,
        Aedit,
        Redit,
        Oedit,
        Nedit,
        Oreset,
        Nreset,
        Alist,
        Rlist,
        Olist,
        Nlist,
        Ashow,
        Rshow,
        Oshow,
        Nshow,
        Goto,
        Clear,
        Reset,
        InvalidCommand
    };
}

#endif //COMMAND_H
