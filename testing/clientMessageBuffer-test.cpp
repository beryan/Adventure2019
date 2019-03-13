//
// Created by louis on 10/03/19.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "ClientMessageBuffer.h"

using networking::Connection;
using networking::Message;
using networking::ClientMessageBuffer;

constexpr Connection CLIENT_A = {100};
constexpr Connection CLIENT_B = {200};

namespace {
    TEST(ClientMessageBufferTestSuite, canAddMessagesToQueues) {
        ClientMessageBuffer clientMessageBuffer;

        std::deque<Message> messages = {
            {CLIENT_A, "first"},
            {CLIENT_B, "first"},
            {CLIENT_A, "second"},
            {CLIENT_A, "third"},
            {CLIENT_B, "second"},
        };

        clientMessageBuffer.addMessagesToQueues(messages);

        auto next = clientMessageBuffer.getNextMessages();
        ASSERT_EQ(2u, next.size());

        next = clientMessageBuffer.getNextMessages();
        ASSERT_EQ(2u, next.size());

        next = clientMessageBuffer.getNextMessages();
        ASSERT_EQ(1u, next.size());

        next = clientMessageBuffer.getNextMessages();
        EXPECT_EQ(0u, next.size());
    }

    TEST(ClientMessageBufferTestSuite, canDiscardDisconnectedClientQueues) {
        ClientMessageBuffer clientMessageBuffer;

        std::deque<Message> messages = {
                {CLIENT_A, "first"},
                {CLIENT_B, "first"},
                {CLIENT_A, "second"},
                {CLIENT_A, "third"},
                {CLIENT_B, "second"},
        };

        clientMessageBuffer.addMessagesToQueues(messages);

        auto next = clientMessageBuffer.getNextMessages();
        ASSERT_EQ(2u, next.size());

        clientMessageBuffer.discardDisconnectedClientQueues({CLIENT_B});

        next = clientMessageBuffer.getNextMessages();
        ASSERT_EQ(1u, next.size());

        clientMessageBuffer.discardDisconnectedClientQueues({CLIENT_A});

        next = clientMessageBuffer.getNextMessages();
        EXPECT_EQ(0u, next.size());
    }
}
