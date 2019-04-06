//
// Created by Stephen Wanhella on 2019-04-02.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "CommandExecutor.h"
#include "CommandParser.h"
#include "Server.h"
#include "Game.h"

namespace {
    class IntegrationTestSuite : public ::testing::Test {
    protected:
        game::CommandParser parser;

        std::vector<Connection> clients;
        std::vector<Connection> newClients;
        std::vector<Connection> disconnectedClients;
        std::function<void(const Connection &)> disconnectClient = [](const Connection &connection) {
            std::cout << "disconnected " << connection.id;
        };
        ConnectionHandler connectionHandler{clients, newClients, disconnectedClients, disconnectClient};
        game::Game game{connectionHandler};

        void SetUp() override {
            clients.clear();
            newClients.clear();
            disconnectedClients.clear();
        }

        std::deque<Message> sendMessage(const Message &message) {
            std::deque<Message> messages;
            messages.push_back(message);
            return game.processCycle(messages);
        };


        struct User {
            std::string username = "stephen";
            std::string password = "asdf";
            std::string gender = "male";
            std::string race = "human";
            std::string trait_1 = "cool";
            std::string trait_2 = "awesome";
            std::string confirmation = "yes";
        };

        User registerUser(Connection connection) {
            User user;

            sendMessage({connection, parser.getStringForCommand(Command::Register)});
            sendMessage({connection, user.username});
            sendMessage({connection, user.password});
            sendMessage({connection, user.password});
            sendMessage({connection, user.gender});
            sendMessage({connection, user.race});
            sendMessage({connection, user.trait_1});
            sendMessage({connection, user.trait_2});
            sendMessage({connection, user.confirmation});

            return user;
        }
    };

    TEST_F(IntegrationTestSuite, canRegisterUser) {
        uintptr_t connectionID = 1;
        Connection connection = {connectionID};
        clients.push_back(connection);

        User user = registerUser(connection);

        std::string lookCommand = parser.getStringForCommand(Command::Look) + " " + user.username;
        const auto &output = sendMessage({connection, lookCommand});
        const Message &lastMessage = output.back();

        EXPECT_EQ(connection, lastMessage.connection);

        std::ostringstream expectedMessage;
        expectedMessage << user.username << " is a " << user.trait_1 << ", " << user.trait_2 << " " << user.gender
                        << " " << user.race << ".\n";
        EXPECT_EQ(expectedMessage.str(), lastMessage.text);
    }

    TEST_F(IntegrationTestSuite, canLoginUser) {
        uintptr_t connectionID = 1;
        Connection connection = {connectionID};
        clients.push_back(connection);

        User user = registerUser(connection);

        sendMessage({connection, parser.getStringForCommand(Command::Logout)});
        sendMessage({connection, parser.getStringForCommand(Command::Login)});
        sendMessage({connection, user.username});
        sendMessage({connection, user.password});

        std::string lookCommand = parser.getStringForCommand(Command::Look) + " " + user.username;
        const auto &output = sendMessage({connection, lookCommand});
        const Message &lastMessage = output.back();

        EXPECT_EQ(connection, lastMessage.connection);

        std::ostringstream expectedMessage;
        expectedMessage << user.username << " is a " << user.trait_1 << ", " << user.trait_2 << " " << user.gender
                        << " " << user.race << ".\n";
        EXPECT_EQ(expectedMessage.str(), lastMessage.text);
    }
}