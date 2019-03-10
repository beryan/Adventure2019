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
        ASSERT_EQ(static_cast<unsigned int>(2), next.size());

        next = clientMessageBuffer.getNextMessages();
        ASSERT_EQ(static_cast<unsigned int>(2), next.size());

        next = clientMessageBuffer.getNextMessages();
        ASSERT_EQ(static_cast<unsigned int>(1), next.size());

        next = clientMessageBuffer.getNextMessages();
        EXPECT_EQ(static_cast<unsigned int>(0), next.size());
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
        ASSERT_EQ(static_cast<unsigned int>(2), next.size());

        clientMessageBuffer.discardDisconnectedClientQueues({CLIENT_B});

        next = clientMessageBuffer.getNextMessages();
        ASSERT_EQ(static_cast<unsigned int>(1), next.size());

        clientMessageBuffer.discardDisconnectedClientQueues({CLIENT_A});

        next = clientMessageBuffer.getNextMessages();
        EXPECT_EQ(static_cast<unsigned int>(0), next.size());
    }
}
