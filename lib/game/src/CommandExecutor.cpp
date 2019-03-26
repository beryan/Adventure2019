//
// Created by Stephen Wanhella on 3/14/2019.
//

#include <CommandExecutor.h>
#include <iostream>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/join.hpp>

using game::CommandExecutor;
using model::NPC;
using model::Room;

constexpr auto ALIAS_LIST = "list";
constexpr auto ALIAS_SET = "set";
constexpr auto ALIAS_SET_GLOBAL = "set-global";
constexpr auto ALIAS_CLEAR = "clear";
constexpr auto ALIAS_CLEAR_GLOBAL = "clear-global";
constexpr auto ALIAS_HELP = "help";
constexpr auto ALIAS_SET_NUM_PARAMS = 3;
constexpr auto ALIAS_CLEAR_NUM_PARAMS = 2;
constexpr auto ALIAS_LIST_NUM_PARAMS = 1;

CommandExecutor::CommandExecutor(ConnectionHandler &connectionHandler, AccountHandler &accountHandler,
                                 MagicHandler &magicHandler, WorldHandler &worldHandler, AliasManager &aliasManager,
                                 CommandParser &commandParser)
        : connectionHandler(connectionHandler),
          accountHandler(accountHandler),
          magicHandler(magicHandler),
          worldHandler(worldHandler),
          aliasManager(aliasManager),
          commandParser(commandParser) {}

std::vector<Message> CommandExecutor::executeCommand(const Connection &client, const Command &command,
                                                     const std::vector<std::string> &params) {
    std::vector<Message> messages = {};
    std::ostringstream tempMessage;

    switch (command) {
        case Command::Logout: {
            tempMessage << logout(client);
            break;
        }

        case Command::Help:
            tempMessage << help();
            break;

        case Command::Say: {
            auto message = boost::algorithm::join(params, " ");
            return say(client, message);
        }

        case Command::Yell: {
            auto message = boost::algorithm::join(params, " ");
            return yell(client, message);
        }

        case Command::Tell: {
            if (params.size() < 2) {
                std::string errorMsg = "Incorrect number of parameters for Tell";
                messages.push_back({client, errorMsg});
                return messages;
            }

            auto username = params[0];
            auto messageRange = std::vector<std::string>(params.begin() + 1, params.end());
            std::string message = boost::algorithm::join(messageRange, " ");

            return tell(client, username, message);
        }

        case Command::Chat: {
            std::string message = boost::algorithm::join(params, " ");
            return chat(client, message);
        }

        case Command::Look: {
            if (params.empty()) {
                tempMessage << look(client);
            } else {
                auto keyword = params[0];
                tempMessage << examine(client, keyword);
            }

            break;
        }

        case Command::Examine: {
            auto keyword = params[0];
            tempMessage << examine(client, keyword);
            break;
        }

        case Command::Exits: {
            tempMessage << exits(client);
            break;
        }

        case Command::Move: {
            auto dir = boost::algorithm::to_lower_copy(params[0]);
            tempMessage << move(client, dir);
            break;
        }

        case Command::Cast: {
            auto spellName = !params.empty() ? params[0] : "";
            auto targetName = params.size() >= 2 ? params[1] : "";
            return magicHandler.castSpell(client, spellName, targetName);
        }

        case Command::Spells: {
            tempMessage << spells();
            break;
        }

        case Command::Talk: {
            auto keyword = params[0];
            tempMessage << talk(client, keyword);
            break;
        }

        case Command::Take: {
            auto keyword = params[0];
            tempMessage << take(client, keyword);
            break;
        }

        case Command::Drop: {
            auto keyword = params[0];
            tempMessage << drop(client, keyword);
            break;
        }

        case Command::Wear: {
            auto keyword = params[0];
            tempMessage << wear(client, keyword);
            break;
        }

        case Command::Remove: {
            auto keyword = params[0];
            tempMessage << remove(client, keyword);
            break;
        }

        case Command::Give: {
            auto username = params[0];
            auto keyword = params[1];
            return give(client, username, keyword);
        }

        case Command::Inventory: {
            tempMessage << inventory(client);
            break;
        }

        case Command::Equipment: {
            tempMessage << equipment(client);
            break;
        }

        case Command::Debug: {
            tempMessage << debug();
            break;
        }

        case Command::Alias: {
            tempMessage << alias(client, params);
            break;
        }

        default: {
            tempMessage << "\nEnter " << "\"" << this->commandParser.getStringForCommand(Command::Help)
                        << "\" for a full list of commands\n";
            break;
        }
    }

    messages.push_back({client, tempMessage.str()});

    return messages;
}

std::vector<Message>
CommandExecutor::tell(const Connection &client, const std::string &username, std::string &message) {
    std::vector<Message> messages;

    if (this->magicHandler.isConfused(client)) {
        this->magicHandler.confuseSpeech(message);
    }

    for (auto connection: connectionHandler.getClients()) {
        auto receiver = this->accountHandler.getUsernameByClient(connection);

        if (receiver == username) {
            auto sender = this->accountHandler.getUsernameByClient(client);

            std::ostringstream toMessage;
            std::ostringstream fromMessage;

            toMessage << "To " << receiver << "> " << message << "\n";
            fromMessage << "From " + sender + "> " << message << "\n";;

            messages.push_back({client, toMessage.str()});
            messages.push_back({connection, fromMessage.str()});

            return messages;
        }
    }

    std::ostringstream tempMessage;
    tempMessage << "Unable to find online user \"" << username << "\".\n";
    messages.push_back({client, tempMessage.str()});
    return messages;
}

std::vector<Message> CommandExecutor::yell(const Connection &client, std::string &message) {
    std::vector<Message> messages;
    auto roomId = accountHandler.getRoomIdByClient(client);
    auto playerIds = worldHandler.getNearbyPlayerIds(roomId);

    for (const auto &playerId : playerIds) {
        auto connection = accountHandler.getClientByPlayerId(playerId);

        if (magicHandler.isConfused(client)) {
            magicHandler.confuseSpeech(message);
        }

        std::ostringstream sayMessage;
        sayMessage << accountHandler.getUsernameByClient(client) << " yells> " << message << "\n";

        messages.push_back({connection, sayMessage.str()});
    }

    return messages;
}

std::string CommandExecutor::debug() {
    std::ostringstream output;
    output << worldHandler.getWorld();
    return output.str();
}

std::string CommandExecutor::equipment(const Connection &client) {
    std::ostringstream output;
    output << accountHandler.getPlayerByClient(client)->getEquipment();
    return output.str();
}

std::string CommandExecutor::inventory(const Connection &client) {
    std::ostringstream output;
    output << accountHandler.getPlayerByClient(client)->getInventory();
    return output.str();
}

std::string CommandExecutor::remove(const Connection &client, const std::string &keyword) {
    std::ostringstream tempMessage;
    auto player = accountHandler.getPlayerByClient(client);
    auto objects = player->getEquipment().getVectorEquipment();

    if (containsKeyword(objects, keyword)) {
        playerHandler.unequipItem(*player, getItemByKeyword(objects, keyword));
        tempMessage << "Item unequipped successfully.\n";
    } else {
        tempMessage << "Invalid keyword.\n";
    }
    return tempMessage.str();
}

std::string CommandExecutor::wear(const Connection &client, const std::string &keyword) {
    std::ostringstream tempMessage;
    auto player = accountHandler.getPlayerByClient(client);
    auto objects = player->getInventory().getVectorInventory();

    if (containsKeyword(objects, keyword)) {
        if (playerHandler.equipItem(*player, getItemByKeyword(objects, keyword))) {
            tempMessage << "Item equipped successfully.\n";
        } else {
            tempMessage << "That item cannot be equipped!\n";
        }
    } else {
        tempMessage << "Invalid keyword.\n";
    }
    return tempMessage.str();
}

std::string CommandExecutor::drop(const Connection &client, const std::string &keyword) {
    std::ostringstream tempMessage;
    auto player = accountHandler.getPlayerByClient(client);
    auto objects = player->getInventory().getVectorInventory();
    auto equip = player->getEquipment().getVectorEquipment();
    objects.insert(objects.end(), equip.begin(), equip.end());

    if (containsKeyword(objects, keyword)) {
        auto item = getItemByKeyword(objects, keyword);
        auto roomId = accountHandler.getRoomIdByClient(client);

        playerHandler.dropItem(*player, item);
        worldHandler.addItem(roomId, item);
        tempMessage << "Item dropped successfully.\n";
    } else {
        tempMessage << "Invalid keyword.\n";
    }
    return tempMessage.str();
}

std::string CommandExecutor::take(const Connection &client, const std::string &keyword) {
    std::ostringstream tempMessage;
    auto roomId = accountHandler.getRoomIdByClient(client);
    Room &room = worldHandler.findRoom(roomId);
    auto objects = room.getObjects();

    if (containsKeyword(objects, keyword)) {
        auto item = getItemByKeyword(objects, keyword);
        auto player = accountHandler.getPlayerByClient(client);
        worldHandler.removeItem(roomId, item.getId());
        playerHandler.pickupItem(*player, item);
        tempMessage << "Item taken successfully.\n";
    } else {
        tempMessage << "Invalid keyword.\n";
    }
    return tempMessage.str();
}

std::string CommandExecutor::talk(const Connection &client, std::string &keyword) {
    std::ostringstream tempMessage;
    auto room = worldHandler.findRoom(accountHandler.getRoomIdByClient(client));
    auto npcs = room.getNpcs();

    if (containsKeyword(npcs, keyword)) {
        auto npc = getItemByKeyword(npcs, keyword);
        for (const auto &str : npc.getLongDescription()) {
            tempMessage << str << std::endl;
        }
    } else {
        tempMessage << "Invalid keyword.\n";
    }
    return tempMessage.str();
}

std::string CommandExecutor::spells() {
    return magicHandler.getSpells();
}

std::string CommandExecutor::move(const Connection &client, const std::string &dir) {
    std::ostringstream tempMessage;
    auto roomId = accountHandler.getRoomIdByClient(client);

    if (worldHandler.isValidDirection(roomId, dir)) {
        auto playerId = accountHandler.getPlayerIdByClient(client);
        auto destinationId = worldHandler.getDestination(roomId, dir);
        worldHandler.movePlayer(playerId, roomId, destinationId);
        accountHandler.setRoomIdByClient(client, destinationId);
        tempMessage << "\n" << worldHandler.findRoom(destinationId).descToString();
    } else {
        tempMessage << "You can't move that way!\n";
    }
    return tempMessage.str();
}

std::string CommandExecutor::examine(const Connection &client, std::string &keyword) {
    std::ostringstream tempMessage;
    auto room = worldHandler.findRoom(accountHandler.getRoomIdByClient(client));
    auto objects = room.getObjects();
    auto npcs = room.getNpcs();
    auto extras = room.getExtras();


    if (containsKeyword(objects, keyword)) {
        auto obj = getItemByKeyword(objects, keyword);
        for (const auto &str : obj.getLongDescription()) {
            tempMessage << str << std::endl;
        }
    } else if (containsKeyword(npcs, keyword)) {
        auto npc = getItemByKeyword(npcs, keyword);
        for (const auto &str : npc.getDescription()) {
            tempMessage << str << std::endl;
        }
    } else if (containsKeyword(extras, keyword)) {
        auto extra = getItemByKeyword(extras, keyword);
        for (const auto &str : extra.getExtraDescriptions()) {
            tempMessage << str << std::endl;
        }
    } else {
        tempMessage << "Invalid keyword.\n";
    }
    return tempMessage.str();
}

std::string CommandExecutor::look(const Connection &client) {
    std::ostringstream tempMessage;
    auto roomId = accountHandler.getRoomIdByClient(client);
    auto room = worldHandler.findRoom(roomId);
    tempMessage << room;
    tempMessage << "[Players]\n";
    for (const auto &id : room.getPlayersInRoom()) {
        tempMessage << accountHandler.getUsernameByPlayerId(id) << std::endl;
    }
    return tempMessage.str();
}

std::vector<Message> CommandExecutor::chat(const Connection &client, std::string message) {
    std::vector<Message> messages = {};
    if (magicHandler.isConfused(client)) {
        magicHandler.confuseSpeech(message);
    }

    for (auto connection : connectionHandler.getClients()) {
        if (accountHandler.isLoggedIn(connection)) {
            std::ostringstream chatMessage;
            chatMessage << accountHandler.getUsernameByClient(client) << "> " << message << "\n";
            messages.push_back({connection, chatMessage.str()});
        }
    }

    return messages;
}

std::string CommandExecutor::exits(const Connection &client) {
    std::ostringstream tempMessage;
    auto roomID = this->accountHandler.getRoomIdByClient(client);
    tempMessage << "\n" << this->worldHandler.findRoom(roomID).doorsToString();
    return tempMessage.str();
}

std::string CommandExecutor::logout(const Connection &client) {
    magicHandler.handleLogout(client);
    removeClientFromGame(client);
    return accountHandler.logoutClient(client);
}

std::vector<Message> CommandExecutor::say(const Connection &client, std::string &message) {
    std::vector<Message> messages = {};
    auto roomId = accountHandler.getRoomIdByClient(client);
    auto playerIds = worldHandler.findRoom(roomId).getPlayersInRoom();

    for (const auto &playerId : playerIds) {
        auto connection = accountHandler.getClientByPlayerId(playerId);

        if (magicHandler.isConfused(client)) {
            magicHandler.confuseSpeech(message);
        }

        std::ostringstream sayMessage;
        sayMessage << accountHandler.getUsernameByClient(client) << " says> " << message << "\n";

        messages.push_back({connection, sayMessage.str()});
    }

    return messages;
}

std::string CommandExecutor::help() {
    std::ostringstream tempMessage;
    tempMessage << "\n"
                << "********\n"
                << "* HELP *\n"
                << "********\n"
                << "\n"
                << "COMMANDS:\n"
                << "  - " << commandParser.getStringForCommand(Command::Help)
                << " (shows this help interface)\n"
                << "  - " << commandParser.getStringForCommand(Command::Chat)
                << " [message] (sends [message] to global chat)\n"
                << "  - " << commandParser.getStringForCommand(Command::Say)
                << " [message] (sends [message] to players in the same room)\n"
                << "  - " << commandParser.getStringForCommand(Command::Yell)
                << " [message] (sends [message] loud enough to be heard by players in adjacent rooms)\n"
                << "  - " << commandParser.getStringForCommand(Command::Tell)
                << " [username] [message] (sends [message] to [username] in the game)\n"
                << "  - " << commandParser.getStringForCommand(Command::Look)
                << " (displays current location information)\n"
                << "  - " << commandParser.getStringForCommand(Command::Exits)
                << " (displays exits from current location)\n"
                << "  - " << commandParser.getStringForCommand(Command::Move)
                << " [direction] (moves you in the direction specified)\n"
                << "  - " << commandParser.getStringForCommand(Command::Examine)
                << " [keyword] (examines something or someone)\n"
                << "  - " << commandParser.getStringForCommand(Command::Talk)
                << " [keyword] (interacts with NPC)\n"
                << "  - " << commandParser.getStringForCommand(Command::Take)
                << " [keyword] (places item in your inventory)\n"
                << "  - " << commandParser.getStringForCommand(Command::Drop)
                << " [keyword] (drops item from inventory/equipment)\n"
                << "  - " << commandParser.getStringForCommand(Command::Wear)
                << " [keyword] (equips item from your inventory)\n"
                << "  - " << commandParser.getStringForCommand(Command::Remove)
                << " [keyword] (unequips item to your inventory)\n"
                << "  - " << commandParser.getStringForCommand(Command::Give)
                << " [username] [keyword] (gives item to username)\n"
                << "  - " << commandParser.getStringForCommand(Command::Inventory)
                << " (displays your inventory)\n"
                << "  - " << commandParser.getStringForCommand(Command::Equipment)
                << " (displays your equipment)\n"
                << "  - " << commandParser.getStringForCommand(Command::Spells)
                << " (displays available magic spells)\n"
                << "  - " << commandParser.getStringForCommand(Command::Cast)
                << " [spell] [target] (casts a spell on a target)\n"
                << "  - " << commandParser.getStringForCommand(Command::Alias)
                << " (aliases a command. Type \""
                << commandParser.getStringForCommand(Command::Alias) << " help\" for details)\n"
                << "  - " << commandParser.getStringForCommand(Command::Logout)
                << " (logs you out of the game)\n"
                << "  - " << commandParser.getStringForCommand(Command::Quit)
                << " (disconnects you from the game server)\n"
                << "  - " << commandParser.getStringForCommand(Command::Shutdown)
                << " (shuts down the game server)\n";
    return tempMessage.str();
}

std::string CommandExecutor::alias(const Connection &client, const std::vector<std::string> &params) {
    std::ostringstream res;
    try {
        std::string username = this->accountHandler.getUsernameByClient(client);
        if (params.empty()) {
            return "\nIncorrect number of parameters for alias command\n";
        }
        std::string aliasOption = params[0];

        if (aliasOption == ALIAS_LIST) {
            if (params.size() != ALIAS_LIST_NUM_PARAMS) {
                return "\nIncorrect number of parameters for alias list command\n";
            }
            auto aliases = this->aliasManager.getAliasesForUser(username);
            auto globalAliases = this->aliasManager.getGlobalAliases();

            res << "\nUser Aliases: \n";
            if (aliases.empty()) {
                res << "\tno user aliases set\n";
            }
            for (const auto &alias: aliases) {
                res << "\t" << alias.first << " -> " << alias.second << std::endl;
            }

            res << "Global Aliases: \n";
            for (const auto &alias: globalAliases) {
                res << "\t" << alias.first << " -> " << alias.second << std::endl;
            }
            if (globalAliases.empty()) {
                res << "\tno global aliases set\n";
            }
        } else if (aliasOption == ALIAS_SET || aliasOption == ALIAS_SET_GLOBAL) {
            if (params.size() != ALIAS_SET_NUM_PARAMS) {
                return "\nIncorrect number of parameters for alias set command\n";
            }

            std::string command_to_alias_str = params[1];
            Command command_to_alias = this->commandParser.parseCommand(command_to_alias_str);

            if (command_to_alias == Command::InvalidCommand) {
                res << "\nAlias could not be set: " << command_to_alias_str
                    << " is not the name of a command\n";
                res.str();
            }

            std::string alias = params[2];

            if (!this->aliasManager.isValidAlias(alias)) {
                res << "\nAlias could not be set: " << alias << " is the name of a default command\n";
                res.str();
            }

            if ((aliasOption == ALIAS_SET &&
                 this->aliasManager.setUserAlias(command_to_alias, alias, username)) ||
                (aliasOption == ALIAS_SET_GLOBAL &&
                 this->aliasManager.setGlobalAlias(command_to_alias, alias))) {
                res << "\nAlias set successfully\n";
            } else {
                res << "\nAlias could not be set: an alias has already been set for the specified command\n";
            }
        } else if (aliasOption == ALIAS_CLEAR || aliasOption == ALIAS_CLEAR_GLOBAL) {
            if (params.size() != ALIAS_CLEAR_NUM_PARAMS) {
                return "\nIncorrect number of parameters for alias clear command\n";
            }
            std::string command_to_clear_str = params[1];
            Command command_to_clear = this->commandParser.parseCommand(command_to_clear_str);

            if (command_to_clear == Command::InvalidCommand) {
                res << "\nAlias could not be cleared: " << command_to_clear_str
                    << " is not the name of a command\n";
                res.str();
            }

            if (aliasOption == ALIAS_CLEAR) {
                this->aliasManager.clearUserAlias(command_to_clear, username);
            } else {
                this->aliasManager.clearGlobalAlias(command_to_clear);
            }

            res << "\nAlias cleared successfully\n";

        } else if (aliasOption.empty() || aliasOption == ALIAS_HELP) {
            res << "\nAlias Commands:\n"
                << "---------------\n"
                << "  - alias list (lists all aliases)\n"
                << "  - alias set [command] [alias] (sets an alias for a command)\n"
                << "  - alias set-global [command] [alias] (sets an alias that will be available to all users)\n"
                << "  - alias clear [command] (clears an alias for a command)\n"
                << "  - alias clear-global [command] (clears a global alias for a command)\n";
        } else {
            res << aliasOption << " is not a valid option for "
                << this->commandParser.getStringForCommand(Command::Alias) << std::endl;
        }
    } catch (std::exception &e) {
        res << "\n error parsing " << this->commandParser.getStringForCommand(Command::Alias)
            << " command!\n";
        std::cout << e.what();
    }

    return res.str();
}

template<typename T>
bool
CommandExecutor::containsKeyword(const std::vector<T> &objects, const std::string &param) {
    auto keyword = boost::algorithm::to_lower_copy(param);
    auto it = std::find_if(objects.begin(), objects.end(),
                           [&keyword](const T &obj) { return obj.containsKeyword(keyword); });
    return (it != objects.end());
}

template<typename T>
T
CommandExecutor::getItemByKeyword(const std::vector<T> &objects, const std::string &param) {
    auto keyword = boost::algorithm::to_lower_copy(param);
    T item;
    auto it = std::find_if(objects.begin(), objects.end(),
                           [&keyword](const T &obj) { return obj.containsKeyword(keyword); });
    if (it != objects.end()) {
        item = *it;
    }
    return item;
}

void
CommandExecutor::removeClientFromGame(Connection client) {
    auto playerId = this->accountHandler.getPlayerIdByClient(client);
    auto roomId = this->accountHandler.getRoomIdByClient(client);
    this->worldHandler.removePlayer(roomId, playerId);
}

std::vector<Message> CommandExecutor::give(const Connection &client, const std::string &username,
                                           const std::string &keyword) {
    std::vector<Message> messages = {};
    auto roomId = this->accountHandler.getRoomIdByClient(client);
    auto receiverClient = this->accountHandler.getClientByUsername(username);
    auto receiverId = this->accountHandler.getPlayerIdByClient(receiverClient);

    auto sender = this->accountHandler.getPlayerByClient(client);
    auto senderName = this->accountHandler.getUsernameByClient(client);
    auto objects = sender->getInventory().getVectorInventory();
    auto equip = sender->getEquipment().getVectorEquipment();
    objects.insert(objects.end(), equip.begin(), equip.end());

    if (!this->worldHandler.canGive(roomId, receiverId) || username == senderName) {
        std::string message = "Invalid username.\n";
        messages.push_back({client, message});
    } else if (containsKeyword(objects, keyword)) {
        auto receiver = this->accountHandler.getPlayerByClient(receiverClient);
        auto object = getItemByKeyword(objects, keyword);
        this->playerHandler.giveItem(*sender, *receiver, object);

        std::ostringstream senderMessage;
        std::ostringstream receiverMessage;

        senderMessage << "Item given successfully.\n";
        receiverMessage << senderName << " has given you " << object.getShortDescription() << std::endl;

        messages.push_back({client, senderMessage.str()});
        messages.push_back({receiverClient, receiverMessage.str()});

        return messages;
    } else {
        std::string message = "Invalid keyword.\n";
        messages.push_back({client, message});
    }

    return messages;
}
