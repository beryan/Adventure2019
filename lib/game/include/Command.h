//
// Created by Stephen Wanhella on 2019-02-14.
//

#ifndef COMMAND_H
#define COMMAND_H

#include "Character.h"

namespace game {
    enum class Command {
        Attack,
        Build,
        Builder,
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

    static std::vector<Command> param = {Command::Say, Command::Yell, Command::Chat,
                                  Command::Move, Command::Examine, Command::Talk,
                                  Command::Take, Command::Drop, Command::Wear,
                                  Command::Remove, Command::Builder};

    static std::vector<Command> build = {Command::Build, Command::Acreate, Command::Rcreate,
                                  Command::Ocreate, Command::Ncreate, Command::Aedit,
                                  Command::Redit, Command::Oedit, Command::Nedit,
                                  Command::Oreset, Command::Nreset, Command::Alist,
                                  Command::Rlist, Command::Olist, Command::Nlist,
                                  Command::Ashow, Command::Rshow, Command::Oshow,
                                  Command::Nshow, Command::Goto, Command::Clear,
                                  Command::Reset};

    static std::vector<Command> admin = {Command::Builder, Command::Shutdown};

    /**
     *  Checks if parameters are incorrect for in-game commands.
     */
    bool isInvalidRole(const Command &command, const model::Role &role);

    /**
     *  Checks if player role is incorrect for in-game commands.
     */
    bool isInvalidFormat(const Command &command, const std::string &parameters);

    /**
     *  Checks if a command requires a role to execute.
     */
    bool isRoleCommand(const Command &command);

}

#endif //COMMAND_H
