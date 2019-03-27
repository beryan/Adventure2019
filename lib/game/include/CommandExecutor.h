//
// Created by Stephen Wanhella on 3/14/2019.
//

#ifndef WEBSOCKETNETWORKING_COMMANDEXECUTOR_H
#define WEBSOCKETNETWORKING_COMMANDEXECUTOR_H

#include <string>
#include <vector>
#include "ConnectionHandler.h"
#include "Command.h"
#include "Server.h"
#include "PlayerHandler.h"
#include "AccountHandler.h"
#include "AvatarHandler.h"
#include "MagicHandler.h"
#include "WorldHandler.h"
#include "AliasManager.h"
#include "CommandParser.h"

using networking::Message;
using handler::PlayerHandler;
using handler::AccountHandler;
using handler::AvatarHandler;
using handler::MagicHandler;
using handler::WorldHandler;
using networking::Connection;

namespace game {
    class CommandExecutor {
    public:
        CommandExecutor(ConnectionHandler &connectionHandler, AccountHandler &accountHandler,
                        AvatarHandler &avatarHandler, MagicHandler &magicHandler, WorldHandler &worldHandler,
                        AliasManager &aliasManager, CommandParser &commandParser);

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
        WorldHandler &worldHandler;
        AliasManager &aliasManager;
        CommandParser &commandParser;

        std::string alias(const Connection &client, const std::vector<std::string> &params);

        std::string help();

        std::vector<Message> say(const Connection &client, std::string &message);

        std::vector<Message> chat(const Connection &client, std::string &message);

        std::string look(const Connection &client);

        std::string examine(const Connection &client, std::string &keyword);

        std::string exits(const Connection &client);

        std::string move(const Connection &client, const std::string &dir);

        std::string spells();

        std::string talk(const Connection &client, std::string &keyword);

        std::string take(const Connection &client, const std::string &keyword);

        std::string drop(const Connection &client, const std::string &keyword);

        std::string wear(const Connection &client, const std::string &keyword);

        std::string remove(const Connection &client, const std::string &keyword);

        std::string inventory(const Connection &client);

        std::string equipment(const Connection &client);

        std::string debug();

        std::vector<Message> give(const Connection &client, const std::string &username, const std::string &keyword);

        template<typename T>
        bool containsKeyword(const std::vector<T> &objects, const std::string &keyword);

        template<typename T>
        T getItemByKeyword(const std::vector<T> &objects, const std::string &keyword);

        std::vector<Message> yell(const Connection &client, std::string &message);

        std::vector<Message> tell(const Connection &client, const std::string &username, std::string &message);
    };
}

#endif //WEBSOCKETNETWORKING_COMMANDEXECUTOR_H
