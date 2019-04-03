//
// Created by Stephen Wanhella on 3/14/2019.
//

#ifndef COMMANDEXECUTOR_H
#define COMMANDEXECUTOR_H

#include <string>
#include <vector>
#include "ConnectionHandler.h"
#include "Command.h"
#include "Server.h"
#include "PlayerHandler.h"
#include "AccountHandler.h"
#include "AvatarHandler.h"
#include "MagicHandler.h"
#include "CombatHandler.h"
#include "WorldHandler.h"
#include "AliasManager.h"
#include "CommandParser.h"

using networking::Message;
using handler::PlayerHandler;
using handler::AccountHandler;
using handler::AvatarHandler;
using handler::MagicHandler;
using handler::CombatHandler;
using handler::WorldHandler;
using networking::Connection;

namespace game {
    class CommandExecutor {
    public:
        CommandExecutor(ConnectionHandler &connectionHandler, AccountHandler &accountHandler,
                        AvatarHandler &avatarHandler, MagicHandler &magicHandler, CombatHandler &combatHandler,
                        WorldHandler &worldHandler, AliasManager &aliasManager, CommandParser &commandParser);

        /**
         * Executes a command
         * @param client client to execute the command for
         * @param command command to execute
         * @param params parameters for the command
         * @return messages for client
         */
        std::vector<Message>
        executeCommand(const Connection &client, const Command &command, const std::vector<std::string> &params);

    private:
        ConnectionHandler &connectionHandler;
        PlayerHandler playerHandler;
        AccountHandler &accountHandler;
        MagicHandler &magicHandler;
        CombatHandler &combatHandler;
        WorldHandler &worldHandler;
        AliasManager &aliasManager;
        CommandParser &commandParser;

        std::string help();

        std::vector<Message> say(const Connection &client, std::string &message);
        std::vector<Message> yell(const Connection &client, std::string &message);
        std::vector<Message> tell(const Connection &client, const std::string &username, std::string &message);
        std::vector<Message> chat(const Connection &client, std::string &message);

        std::string look(const Connection &client);
        std::string examine(const Connection &client, const std::string &keyword);
        std::string exits(const Connection &client);
        std::string move(const Connection &client, const std::string &dir);

        std::string talk(const Connection &client, const std::string &keyword);
        std::string take(const Connection &client, const std::string &keyword);
        std::string drop(const Connection &client, const std::string &keyword);
        std::string wear(const Connection &client, const std::string &keyword);
        std::string remove(const Connection &client, const std::string &keyword);
        std::vector<Message> give(const Connection &client, const std::string &username, const std::string &keyword);

        std::string status(const Connection &client);
        std::string alias(const Connection &client, const std::vector<std::string> &params);

        std::string build();
        std::string alist();
        std::string rlist(const std::string &param);
        std::string olist(const std::string &param);
        std::string nlist(const std::string &param);
        std::string oshow(const Connection &client, const std::string &param);
        std::string nshow(const Connection &client, const std::string &param);
        std::string go(const Connection &client, const std::string &param);

        template<typename T>
        bool containsKeyword(const std::vector<T> &objects, const std::string &keyword);

        template<typename T>
        T getItemByKeyword(const std::vector<T> &objects, const std::string &keyword);
    };
}

#endif //COMMANDEXECUTOR_H
