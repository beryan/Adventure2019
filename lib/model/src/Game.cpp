//
// Created by louis on 19/01/19.
//

#include "Game.h"

#include <map>
#include <sstream>
#include <iostream>
#include <boost/algorithm/string.hpp>

using model::Game;
using model::AccountHandler;
using model::WorldHandler;

std::string
lowercase(std::string string) {
    std::transform(string.begin(), string.end(), string.begin(), ::tolower);
    return string;
}

std::string
trimWhitespace(const std::string &string) {
    std::string whitespace = " \t";
    auto start = string.find_first_not_of(whitespace);

    if (start == std::string::npos) {
        return "";
    }

    auto end = string.find_last_not_of(whitespace);
    auto size = end - start + 1;

    return string.substr(start, size);
}

namespace model {
    // move these when refactoring out commands
    constexpr auto ALIAS_LIST = "list";
    constexpr auto ALIAS_SET = "set";
    constexpr auto ALIAS_CLEAR = "clear";
    constexpr auto ALIAS_HELP = "help";

    Game::Game(std::vector<Connection> &clients,
               std::vector<Connection> &newClients,
               std::vector<Connection> &disconnectedClients,
               std::function<void(Connection)> &disconnect,
               std::function<void()> &shutdown) {
        this->clients = &clients;
        this->newClients = &newClients;
        this->disconnectedClients = &disconnectedClients;

        this->disconnect = disconnect;
        this->shutdown = shutdown;

        this->accountHandler = std::make_unique<AccountHandler>();
        this->worldHandler = std::make_unique<WorldHandler>();
    }


    void
    Game::handleConnects(std::deque<Message> &messages) {
        for (auto &newClient : *this->newClients) {
            std::ostringstream introduction;

            introduction << "Welcome to Adventure 2019!\n"
                         << "\n"
                         << "Enter " << "\"" << this->commandHandler.getStringForCommand(Command::Login) << "\" to login to an existing account\n"
                         << "Enter " << "\"" << this->commandHandler.getStringForCommand(Command::Register) << "\" to create a new account\n";

            messages.push_back({newClient, introduction.str()});
        }

        this->newClients->clear();
    }


    void
    Game::handleDisconnects(std::deque<Message> &messages) {

        for (auto &disconnectedClient : *this->disconnectedClients) {
            if (this->accountHandler->isLoggingIn(disconnectedClient)) {
                this->accountHandler->exitLogin(disconnectedClient);
                std::cout << disconnectedClient.id << " has been removed from login due to disconnect\n";
            }

            if (this->accountHandler->isRegistering(disconnectedClient)) {
                this->accountHandler->exitRegistration(disconnectedClient);
                std::cout << disconnectedClient.id << " has been removed from registration due to disconnect\n";
            }

            if (this->accountHandler->isLoggedIn(disconnectedClient)) {
                this->removeClientFromGame(disconnectedClient);
                this->accountHandler->logoutClient(disconnectedClient);
                std::cout << disconnectedClient.id << " has been logged out of the game due to disconnect\n";
            }
        }

        this->disconnectedClients->clear();
    }


    void
    Game::handleIncoming(const std::deque<Message> &incoming, std::deque<Message> &messages) {
        for (const auto& input : incoming) {
            auto client = input.connection;
            auto incomingInput = trimWhitespace(input.text);
            std::ostringstream tempMessage;

            if (this->accountHandler->isLoggingIn(client)) {
                messages.push_back({
                    client,
                    this->accountHandler->processLogin(client, incomingInput.substr(0, incomingInput.find(' ')))
                });

                if (this->accountHandler->isLoggedIn(client)) {
                    this->addClientToGame(client);
                    auto roomID = this->accountHandler->getRoomIdByClient(client);
                    tempMessage << "\n" << this->worldHandler->findRoom(roomID).descToString();
                    messages.push_back({client, tempMessage.str()});
                }

                continue;

            } else if (this->accountHandler->isRegistering(client)) {
                messages.push_back({
                    client,
                    this->accountHandler->processRegistration(client, incomingInput.substr(0, incomingInput.find(' ')))
                });

                if (this->accountHandler->isLoggedIn(client)) {
                    this->addClientToGame(client);
                    auto roomID = this->accountHandler->getRoomIdByClient(client);
                    tempMessage << "\n" << this->worldHandler->findRoom(roomID).descToString();
                    messages.push_back({client, tempMessage.str()});
                }

                continue;
            }

            std::string commandString = lowercase(incomingInput.substr(0, incomingInput.find(' ')));
            std::string username = this->accountHandler->getUsernameByClient(client);
            Command command = this->commandHandler.getCommandForUser(commandString, username);

            if (command == Command::InvalidCommand) {
                tempMessage << "The word \"" << commandString << "\" is not a valid command.\n";
                messages.push_back({client, tempMessage.str()});
                continue;
            }

            std::string parameters;

            if (incomingInput.find(' ') != std::string::npos) {
                parameters = trimWhitespace(incomingInput.substr(incomingInput.find(' ') + 1));
            }

            switch (command) {
                case Command::Quit: {
                    this->disconnect(input.connection);
                    continue;
                }

                case Command::Shutdown: {
                    std::cout << "Shutting down.\n";
                    this->shutdown();
                    return;
                }

                default:
                    break;
            }

            if (!this->accountHandler->isLoggedIn(client)) {
                messages.push_back(this->executeMenuAction(client, command, parameters));
            } else {
                if (this->isInvalidFormat(command, parameters)) {
                    tempMessage << "Invalid format for command \"" << commandString << "\".\n";
                    messages.push_back({client, tempMessage.str()});
                    continue;
                }
                auto responseList = this->executeInGameAction(client, command, parameters);

                for (auto &response : responseList) {
                    messages.push_back(response);
                }
            }
        }
    }

    Message
    Game::executeMenuAction(const Connection &client,
                            const Command &command,
                            const std::string &param) {
        std::ostringstream tempMessage;

        switch (command) {
            case Command::Register:
                tempMessage << this->accountHandler->processRegistration(client);
                break;

            case Command::Login:
                tempMessage << this->accountHandler->processLogin(client);
                break;

            case Command::Help:
                tempMessage << "\n"
                            << "********\n"
                            << "* HELP *\n"
                            << "********\n"
                            << "\n"
                            << "COMMANDS:\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Help) << " (shows this help interface)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Register) << " (create a new account)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Login) << " (login to an existing account)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Quit) << " (disconnects you from the game server)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Shutdown) << " (shuts down the game server)\n";
                break;

            default:
                tempMessage << "\n"
                            << "Enter " << "\"" << this->commandHandler.getStringForCommand(Command::Login) << "\" to login to an existing account\n"
                            << "Enter " << "\"" << this->commandHandler.getStringForCommand(Command::Register) << "\" to create a new account\n"
                            << "Enter " << "\"" << this->commandHandler.getStringForCommand(Command::Help) << "\" for a full list of commands\n";
                break;
        }

        return {client, tempMessage.str()};
    }

    std::vector<Message>
    Game::executeInGameAction(const Connection &client,
                              const Command &command,
                              const std::string &param) {
        std::vector<Message> messages = {};
        std::ostringstream tempMessage;

        std::vector<std::string> params;
        boost::split(params, param, boost::is_any_of("\t "));

        switch (command) {
            case Command::Logout: {
                this->removeClientFromGame(client);
                tempMessage << this->accountHandler->logoutClient(client);
                break;
            }

            case Command::Help:
                tempMessage << "\n"
                            << "********\n"
                            << "* HELP *\n"
                            << "********\n"
                            << "\n"
                            << "COMMANDS:\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Help) << " (shows this help interface)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Say) << " [message] (sends [message] to nearby players in the game)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Tell) << " [username] [message] (sends [message] to [username] in the game)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Yell) << " [message] (sends [message] to other players in the game)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Look) << " (displays current location description)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Info) << " (displays current location information)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Exits) << " (displays exits from current location)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Move) << " [direction] (moves you in the direction specified)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Logout) << " (logs you out of the game)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Quit) << " (disconnects you from the game server)\n"
                            << "  - " << this->commandHandler.getStringForCommand(Command::Shutdown) << " (shuts down the game server)\n";
                break;

            case Command::Say: {
                auto roomId = this->accountHandler->getRoomIdByClient(client);
                auto playerIds = this->worldHandler->getNearbyPlayerIds(roomId);

                for (auto playerId : playerIds) {
                    auto connection = this->accountHandler->getClientByPlayerId(playerId);
                    std::ostringstream sayMessage;
                    sayMessage << this->accountHandler->getUsernameByClient(client) << "> " << param << "\n";
                    messages.push_back({connection, sayMessage.str()});
                }

                return messages;
            }

            case Command::Tell: {
                auto username = param.substr(0, param.find(' '));
                auto message = trimWhitespace(param.substr(param.find(' ') + 1));

                for (auto connection: *this->clients) {
                    auto receiver = this->accountHandler->getUsernameByClient(connection);

                    if (receiver == username)  {
                        auto sender = this->accountHandler->getUsernameByClient(client);

                        std::ostringstream toMessage;
                        std::ostringstream fromMessage;

                        toMessage << "To " << receiver << "> " << message << "\n";
                        fromMessage << "From " + sender + "> " << message << "\n";;

                        messages.push_back({client, toMessage.str()});
                        messages.push_back({connection, fromMessage.str()});

                        return messages;
                    }
                }

                tempMessage << "Unable to find online user \"" << username << "\".\n";

                break;
            }

            case Command::Yell: {
                for (auto connection : *this->clients) {
                    std::ostringstream yellMessage;
                    yellMessage << this->accountHandler->getUsernameByClient(client) << "> " << param << "\n";
                    messages.push_back({connection, yellMessage.str()});
                }

                return messages;
            }

            case Command::Look: {
                auto roomID = this->accountHandler->getRoomIdByClient(client);
                tempMessage << "\n" << this->worldHandler->findRoom(roomID).descToString();
                break;
            }

            case Command::Info: {
                auto roomID = this->accountHandler->getRoomIdByClient(client);
                tempMessage << this->worldHandler->findRoom(roomID);
                break;
            }

            case Command::Exits: {
                auto roomID = this->accountHandler->getRoomIdByClient(client);
                tempMessage << "\n" << this->worldHandler->findRoom(roomID).doorsToString();
                break;
            }

            case Command::Move: {
                auto roomID = this->accountHandler->getRoomIdByClient(client);

                if (this->worldHandler->isValidDirection(roomID, param)) {
                    auto playerID = this->accountHandler->getPlayerIdByClient(client);
                    auto destinationID = this->worldHandler->getDestination(roomID, param);
                    this->worldHandler->movePlayer(playerID, roomID, destinationID);
                    this->accountHandler->setRoomIdByClient(client, destinationID);
                    tempMessage << "\n" << this->worldHandler->findRoom(destinationID).descToString();

                } else {
                    tempMessage << "You can't move that way!\n";
                }

                break;
            }

            case Command::Debug: {
                tempMessage << this->worldHandler->getWorld();
                break;
            }

            case Command::Alias: {
                try {
                    std::string username = this->accountHandler->getUsernameByClient(client);
                    std::string aliasOption = params[0];

                    if (aliasOption == ALIAS_LIST) {
                        auto aliases = this->commandHandler.getAliasesForUser(username);
                        auto globalAliases = this->commandHandler.getGlobalAliases();

                        tempMessage << "\nUser Aliases: \n";
                        if (aliases.empty()) {
                            tempMessage << "\tno user aliases set\n";
                        }
                        for (const auto &alias: aliases) {
                            tempMessage << "\t" << alias.first << " -> " << alias.second << std::endl;
                        }

                        tempMessage << "Global Aliases: \n";
                        for (const auto &alias: globalAliases) {
                            tempMessage << "\t" << alias.first << " -> " << alias.second << std::endl;
                        }
                        if (globalAliases.empty()) {
                            tempMessage << "\tno global aliases set\n";
                        }
                    } else if (aliasOption == ALIAS_SET) {
                        std::string command_to_alias_str = params[1];
                        Command command_to_alias = this->commandHandler.getDefaultCommand(command_to_alias_str);
                        if (command_to_alias != Command::InvalidCommand) {
                            std::string alias = params[2];
                            this->commandHandler.setUserAlias(command_to_alias, alias, username);
                            tempMessage << "\nalias set successfully\n";
                        } else {
                            tempMessage << std::endl << command_to_alias_str << " did not map to a command\n";
                        }
                    } else if (aliasOption == ALIAS_CLEAR) {
                        std::string command_to_clear_str = params[1];
                        Command command_to_clear = this->commandHandler.getDefaultCommand(command_to_clear_str);
                        if (command_to_clear != Command::InvalidCommand) {
                            this->commandHandler.clearUserAlias(command_to_clear, username);
                            tempMessage << "\nalias cleared successfully\n";
                        } else {
                            tempMessage << std::endl << command_to_clear_str << " did not map to a command\n";
                        }
                    } else if (aliasOption.empty() || aliasOption == ALIAS_HELP) {
                        tempMessage << "\nalias help: \n";
                        tempMessage << "\talias list: list all aliases\n";
                        tempMessage << "\talias set [command to alias] [alias]: sets an alias\n";
                        tempMessage << "\talias clear [aliased command]: clear an alias for a command\n";
                    } else {
                        tempMessage << aliasOption << " is not a valid option for "
                                    << this->commandHandler.getStringForCommand(Command::Alias) << std::endl;
                    }
                } catch (std::exception &e) {
                    tempMessage << "\n error parsing " << this->commandHandler.getStringForCommand(Command::Alias)
                            << " command!\n";
                    std::cout << e.what();
                }

                break;
            }

            default:
                tempMessage << "\nEnter " << "\"" << this->commandHandler.getStringForCommand(Command::Help) << "\" for a full list of commands\n";
                break;
        }

        messages.push_back({client, tempMessage.str()});

        return messages;
    }


    void
    Game::handleOutgoing(std::deque<Message> &messages) {
        this->accountHandler->notifyBootedClients(messages);
    }


    std::deque<Message>
    Game::formMessages(std::deque<Message> &messages) {
        std::deque<Message> outgoing;
        std::map<Connection, std::ostringstream> clientMessages;

        for (const auto &message : messages) {
            clientMessages[message.connection] << message.text;
        }

        for (auto const& [client, message] : clientMessages) {
            outgoing.push_back({client, message.str()});
        }

        return outgoing;
    }


    void
    Game::addClientToGame(Connection client) {
        auto playerID = this->accountHandler->getPlayerIdByClient(client);
        auto roomID = this->accountHandler->getRoomIdByClient(client);

        this->worldHandler->addPlayer(playerID, roomID);
    }


    void
    Game::removeClientFromGame(Connection client) {
        auto playerID = this->accountHandler->getPlayerIdByClient(client);
        auto roomID = this->accountHandler->getRoomIdByClient(client);
        this->worldHandler->removePlayer(playerID, roomID);
    }


    bool
    Game::isInvalidFormat(const Command &command, const std::string &parameters) {
        bool wrongTellFormat = (command == Command::Tell && parameters.find(' ') == std::string::npos);
        bool isCommandWithParam = (command == Command::Move || command == Command::Say  || command == Command::Yell);

        return (wrongTellFormat || (isCommandWithParam && parameters.empty()));
    }


    std::deque<Message>
    Game::processCycle(std::deque<Message> &incoming) {
        std::deque<Message> messages;

        this->handleConnects(messages);
        this->handleDisconnects(messages);
        this->handleIncoming(incoming, messages);
        this->handleOutgoing(messages);

        return this->formMessages(messages);
    }
}
