//
// Created by louis on 07/02/19.
//

#include "lib/gtest/gtest.h"
#include "lib/gmock/gmock.h"
#include "Game.h"
#include "Response.h"
#include "Server.h"

using model::Game;
using model::Response;
using networking::Connection;
using networking::Message;

/*
 *  Cases to test:
 *  1. Constructor can create instance with provided arguments
 *  2. Get greeting message on connection
 *  3. Pass-by-reference disconnect function works
 *  4. Pass-by-reference shutdown function works
 */
const Connection CLIENT_A = {100};
const std::string DISCONNECT_COMMAND = "quit";
const std::string SHUTDOWN_COMMAND = "shutdown";

TEST(GameTestSuite, canConstructGameInstance) {
    std::vector<Connection> clients;
    std::vector<Connection> newClients;
    std::vector<Connection> disconnectedClients;
    std::function<void(Connection)> disconnect = [](Connection) {};
    std::function<void()> shutdown = []() {};

    Game game{clients, newClients, disconnectedClients, disconnect, shutdown};


    unsigned int expectedValue = 0;
    std::deque<Message> userInput{};

    EXPECT_EQ(expectedValue, game.processCycle(userInput).size());
}

TEST(GameTestSuite, canGetGreetingOnConnect) {
    std::vector<Connection> clients{CLIENT_A};
    std::vector<Connection> newClients{CLIENT_A};
    std::vector<Connection> disconnectedClients;
    std::function<void(Connection)> disconnect = [](Connection) { };
    std::function<void()> shutdown = []() {};


    Game game{clients, newClients, disconnectedClients, disconnect, shutdown};

    std::ostringstream introduction;
    introduction << "Welcome to Adventure 2019!\n"
                 << "\n"
                 << "Enter \"login\" to sign into an existing account\n"
                 << "Enter \"register\" to make a new account\n";

    Message expectedMessage = {CLIENT_A, introduction.str()};
    unsigned int expectedNewClientsSize = 0;

    std::deque<Message> userInput;
    auto result = game.processCycle(userInput);

    // Greeting should be sent back to connected client and client is removed from newClients vector
    EXPECT_EQ(expectedMessage.connection, result.front().connection);
    EXPECT_EQ(expectedMessage.text, result.front().text);
    EXPECT_EQ(expectedNewClientsSize, newClients.size());

    // Greeting should not be resent to client in following cycles
    userInput.clear();
    result = game.processCycle(userInput);
    unsigned int expectedNumberOfMessages = 0;
    EXPECT_EQ(expectedNumberOfMessages, result.size());
}

TEST(GameTestSuite, canCallDisconnectCommand) {
    bool isDisconnected = false;

    std::vector<Connection> clients{CLIENT_A};
    std::vector<Connection> newClients;
    std::vector<Connection> disconnectedClients;
    std::function<void(Connection)> disconnect = [&isDisconnected](Connection) {
        isDisconnected = true;
    };
    std::function<void()> shutdown = []() {};

    Game game{clients, newClients, disconnectedClients, disconnect, shutdown};
    std::deque<Message> userInput{{CLIENT_A, DISCONNECT_COMMAND}};

    game.processCycle(userInput);

    unsigned int expectedDisconnectedClientsSize = 0;

    EXPECT_EQ(true, isDisconnected);
    EXPECT_EQ(expectedDisconnectedClientsSize, disconnectedClients.size());
}

TEST(GameTestSuite, canCallShutdownCommand) {
    bool isShutdown= false;

    std::vector<Connection> clients{CLIENT_A};
    std::vector<Connection> newClients;
    std::vector<Connection> disconnectedClients;
    std::function<void(Connection)> disconnect = [](Connection) {};
    std::function<void()> shutdown = [&isShutdown]() {
        isShutdown = true;
    };

    Game game{clients, newClients, disconnectedClients, disconnect, shutdown};
    std::deque<Message> userInput{{CLIENT_A, SHUTDOWN_COMMAND}};

    game.processCycle(userInput);

    EXPECT_EQ(true, isShutdown);
}
