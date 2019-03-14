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
                {"cast",      Command::Cast},
                {"chat",      Command::Chat},
                {"debug",     Command::Debug},
                {"drop",      Command::Drop},
                {"equipment", Command::Equipment},
                {"examine",   Command::Examine},
                {"exits",     Command::Exits},
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
                {"yell",      Command::Yell}
        };
    };
}


#endif //WEBSOCKETNETWORKING_COMMANDPARSER_H
