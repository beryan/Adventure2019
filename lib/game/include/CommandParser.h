//
// Created by Stephen Wanhella on 2019-03-12.
//

#ifndef WEBSOCKETNETWORKING_COMMANDPARSER_H
#define WEBSOCKETNETWORKING_COMMANDPARSER_H

#include "Command.h"
#include <string>
#include <unordered_map>

namespace game {
    class CommandParser {
    public:
        /**
         * Gets default command from string
         * @param commandStr command string to parse
         * @return Command that matches commandStr. InvalidCommand if no matching command
         */
        Command parseCommand(std::string_view commandStr);

        /**
         * Returns the default string that represents a command
         * @param command command to get the string for
         * @return the default string for a command
         */
        std::string getStringForCommand(const Command &command);

    private:
        std::unordered_map<std::string, Command> commands = {
                {"alias",     Command::Alias},
                {"cast",      Command::Cast},
                {"chat",      Command::Chat},
                {"build",     Command::Build},
                {"drop",      Command::Drop},
                {"equipment", Command::Equipment},
                {"examine",   Command::Examine},
                {"exits",     Command::Exits},
                {"give",      Command::Give},
                {"help",      Command::Help},
                {"inventory", Command::Inventory},
                {"login",     Command::Login},
                {"logout",    Command::Logout},
                {"look",      Command::Look},
                {"move",      Command::Move},
                {"quit",      Command::Quit},
                {"register",  Command::Register},
                {"remove",    Command::Remove},
                {"say",       Command::Say},
                {"shutdown",  Command::Shutdown},
                {"spells",    Command::Spells},
                {"take",      Command::Take},
                {"talk",      Command::Talk},
                {"tell",      Command::Tell},
                {"wear",      Command::Wear},
                {"yell",      Command::Yell},

                {"acreate",   Command::Acreate},
                {"rcreate",   Command::Rcreate},
                {"ocreate",   Command::Ocreate},
                {"ncreate",   Command::Ncreate},
                {"aedit",     Command::Aedit},
                {"redit",     Command::Redit},
                {"oedit",     Command::Oedit},
                {"nedit",     Command::Nedit},
                {"oreset",    Command::Oreset},
                {"nreset",    Command::Nreset},
                {"alist",     Command::Alist},
                {"rlist",     Command::Rlist},
                {"olist",     Command::Olist},
                {"nlist",     Command::Nlist},
                {"ashow",     Command::Ashow},
                {"rshow",     Command::Rshow},
                {"oshow",     Command::Oshow},
                {"nshow",     Command::Nshow},
                {"goto",      Command::Goto},
                {"clear",     Command::Clear},
                {"reset",     Command::Reset}
        };
    };
}


#endif //WEBSOCKETNETWORKING_COMMANDPARSER_H
