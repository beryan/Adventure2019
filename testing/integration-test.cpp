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
    };

    TEST_F(IntegrationTestSuite, canRegisterUser) {
        uintptr_t connectionID = 1;
        Connection connection = {connectionID};
        clients.push_back(connection);

        std::string username = "stephen";
        std::string password = "asdf";
        std::string gender = "male";
        std::string race = "human";
        std::string trait_1 = "cool";
        std::string trait_2 = "awesome";
        std::string confirmation = "yes";

        sendMessage({connection, parser.getStringForCommand(Command::Register)});
        sendMessage({connection, username});
        sendMessage({connection, password});
        sendMessage({connection, password});
        sendMessage({connection, gender});
        sendMessage({connection, race});
        sendMessage({connection, trait_1});
        sendMessage({connection, trait_2});
        sendMessage({connection, confirmation});

        const auto &output = sendMessage({connection, parser.getStringForCommand(Command::Look) + " " + username});
        const Message &lastMessage = output.back();

        EXPECT_EQ(connection, lastMessage.connection);

        std::ostringstream expectedMessage;
        expectedMessage << username << " is a " << trait_1 << ", " << trait_2 << " " << gender << " " << race << ".\n";
        EXPECT_EQ(expectedMessage.str(), lastMessage.text);
    }
}