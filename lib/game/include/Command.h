//
// Created by Stephen Wanhella on 2019-02-14.
//

#ifndef COMMAND_H
#define COMMAND_H
namespace game {
    enum class Command {
        Build,
        Cast,
        Chat,
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
        InvalidCommand,

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
        Goto,
        Reset,
    };
}

#endif //COMMAND_H
