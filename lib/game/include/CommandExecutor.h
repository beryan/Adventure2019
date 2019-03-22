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
#include "MagicHandler.h"
#include "WorldHandler.h"
#include "AliasManager.h"
#include "CommandParser.h"

using networking::Message;
using handler::PlayerHandler;
using handler::AccountHandler;
using handler::MagicHandler;
using handler::WorldHandler;
using networking::Connection;

namespace game {
    class CommandExecutor {
    public:
        CommandExecutor(ConnectionHandler &connectionHandler, AccountHandler &accountHandler,
                        MagicHandler &magicHandler,
                        WorldHandler &worldHandler, AliasManager &aliasManager, CommandParser &commandParser);

        /**
         * Executes a command
         * @param client client to execute the command for
         * @param command command to execute
         * @param params parameters for the command
         * @return messages for client
         */
        std::vector<Message> executeCommand(const Connection &client, const Command &command,
                                            const std::vector<std::string> &params);

        /**
         *  Update game state to not include connections that are no longer in game.
         */
        void removeClientFromGame(Connection client);

    private:
        ConnectionHandler &connectionHandler;
        PlayerHandler playerHandler;
        AccountHandler accountHandler;
        MagicHandler magicHandler;
        WorldHandler worldHandler;
        AliasManager aliasManager;
        CommandParser commandParser;

        std::string executeAliasCommand(const Connection &client, const std::vector<std::string> &params);

        template<typename T>
        bool
        containsKeyword(const std::vector<T> &objects, const std::string &keyword);

        template<typename T>
        T
        getItemByKeyword(const std::vector<T> &objects, const std::string &keyword);

        std::string executeHelpCommand();

        std::vector<Message> executeSayCommand(const Connection &client, std::string &message);

        std::string executeLogoutCommand(const Connection &client);

        std::vector<Message> executeChatCommand(const Connection &client, std::basic_string<char> &message);

        std::string executeLookCommand(const Connection &client);

        std::string executeExamineCommand(const Connection &client, std::string &keyword);

        std::string executeExitsCommand(const Connection &client);

        std::string executeMoveCommand(const Connection &client, const std::string &dir);

        std::vector<Message> executeCastCommand(const Connection &client, const std::string &spell);

        std::string executeSpellsCommand();

        std::string executeTalkCommand(const Connection &client, std::string &keyword);

        std::string executeTakeCommand(const Connection &client, const std::string &keyword);

        std::string executeDropCommand(const Connection &client, const std::string &keyword);

        std::string executeWearCommand(const Connection &client, const std::string &keyword);

        std::string executeRemoveCommand(const Connection &client, const std::string &keyword);

        Inventory &executeInventoryCommand(const Connection &client);

        Equipment &executeEquipmentCommand(const Connection &client);

        World executeDebugCommand();

        std::vector<Message>
        executeGiveCommand(const Connection &client, const std::string &username, const std::string &keyword);
    };
}

#endif //WEBSOCKETNETWORKING_COMMANDEXECUTOR_H
