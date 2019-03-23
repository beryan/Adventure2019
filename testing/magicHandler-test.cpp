//
// Created by louis on 21/02/19.
//

#include "lib/gtest/gtest.h"
#include "lib/gmock/gmock.h"
#include "MagicHandler.h"
#include "AccountHandler.h"
#include "Server.h"

using handler::MagicHandler;
using handler::AccountHandler;
using networking::Connection;


constexpr Connection CLIENT_A = {100};
constexpr Connection CLIENT_B = {200};
constexpr Connection CLIENT_C = {300};

constexpr auto USERNAME_A = "Able";
constexpr auto USERNAME_B = "Baker";
constexpr auto USERNAME_C = "Charlie";

constexpr auto VALID_PASSWORD_STRING = "Valid Pass";

constexpr auto CONFUSE_SPELL_NAME = "confuse";
constexpr auto BODY_SWAP_SPELL_NAME = "swap";

constexpr unsigned int CONFUSE_DURATION = 50;
constexpr unsigned int BODY_SWAP_DURATION = 50;


namespace {
    class MagicHandlerTestSuite : public ::testing::Test {
    protected:
        AccountHandler accountHandler;
        MagicHandler magicHandler = MagicHandler{accountHandler};

        virtual void SetUp() override {
            // Register client A
            accountHandler.processRegistration(CLIENT_A);
            accountHandler.processRegistration(CLIENT_A, USERNAME_A);
            accountHandler.processRegistration(CLIENT_A, VALID_PASSWORD_STRING);
            accountHandler.processRegistration(CLIENT_A, VALID_PASSWORD_STRING);

            // Register client B
            accountHandler.processRegistration(CLIENT_B);
            accountHandler.processRegistration(CLIENT_B, USERNAME_B);
            accountHandler.processRegistration(CLIENT_B, VALID_PASSWORD_STRING);
            accountHandler.processRegistration(CLIENT_B, VALID_PASSWORD_STRING);
        }
    };

    /*
     *  Cases to test:
     *  1. Reject if no spell name
     *  2. Reject invalid spell names
     *  3. A player can cast Confuse on themselves
     *  4. A player can cast Confuse on another player in the same room
     *  5. A player cannot cast Confuse on another player in a different room
     *  6. A player cannot cast Confuse on themselves if they are already Confused
     *  7. A player cannot cast Confuse on an already Confused player
     *  8. The confuseSpeech() method can convert a string into Pig Latin
     *  9. Confuse spell instance will expire after a certain number of cycles
     * 10. Confuse is removed on logout
     * 11. A player cannot cast Body Swap on themselves
     * 12. A player can cast Body Swap on another player in the same room
     * 13. A player cannot cast Body Swap on another player in a different room
     * 14. A player cannot cast Body Swap if they are under the Body Swap spell's effects
     * 15. A player cannot cast Body Swap on an already Body Swapped player
     * 16. Body Swap spell instance will expire after a certain number of cycles
     * 17. Body Swap is removed on logout
     * 18. Can get spells list
     */


    TEST_F(MagicHandlerTestSuite, rejectNoSpellName) {
        auto results = magicHandler.castSpell(CLIENT_A, "", "");

        ASSERT_EQ(1u, results.size());

        auto result = results.back();

        std::string casterExpected = "You need to specify the name of the spell to be cast.\n";

        EXPECT_EQ(CLIENT_A.id, result.connection.id);
        EXPECT_EQ(casterExpected, result.text);
    }


    TEST_F(MagicHandlerTestSuite, rejectInvalidSpellName) {
        std::string spellName = "InvalidSpell";
        std::string targetName = USERNAME_B;

        auto results = magicHandler.castSpell(CLIENT_A, spellName, targetName);

        ASSERT_EQ(1u, results.size());

        auto result = results.back();

        std::ostringstream casterExpected;
        casterExpected << "There are no spells with the name of \"" << spellName << "\"\n";

        EXPECT_EQ(CLIENT_A.id, result.connection.id);
        EXPECT_EQ(casterExpected.str(), result.text);
    }


    TEST_F(MagicHandlerTestSuite, canCastConfuseOnSelf) {
        std::string spellName = CONFUSE_SPELL_NAME;
        std::string targetName = USERNAME_A;
        auto results = magicHandler.castSpell(CLIENT_A, spellName, targetName);

        ASSERT_EQ(1u, results.size());

        auto result = results.back();

        std::ostringstream casterExpected;
        casterExpected << "You cast " << CONFUSE_SPELL_NAME << " on yourself.\n";

        EXPECT_TRUE(magicHandler.isConfused(CLIENT_A));
        EXPECT_FALSE(magicHandler.isConfused(CLIENT_B));

        EXPECT_EQ(CLIENT_A.id, result.connection.id);
        EXPECT_EQ(casterExpected.str(), result.text);
    }


    TEST_F(MagicHandlerTestSuite, canCastConfuseOnOtherPlayerInSameRoom) {
        ASSERT_EQ(accountHandler.getRoomIdByClient(CLIENT_A), accountHandler.getRoomIdByClient(CLIENT_B));

        std::string spellName = CONFUSE_SPELL_NAME;
        std::string targetName = USERNAME_B;
        auto results = magicHandler.castSpell(CLIENT_A, spellName, targetName);

        ASSERT_EQ(2u, results.size());

        auto casterResult = results.front();
        auto targetResult = results.back();

        std::ostringstream casterExpected;
        casterExpected << "You cast " << CONFUSE_SPELL_NAME << " on " << USERNAME_B << "\n";

        std::ostringstream targetExpected;
        targetExpected << USERNAME_A << " casts " << CONFUSE_SPELL_NAME << " on you!" << "\n";

        EXPECT_FALSE(magicHandler.isConfused(CLIENT_A));
        EXPECT_TRUE(magicHandler.isConfused(CLIENT_B));

        EXPECT_EQ(CLIENT_A.id, casterResult.connection.id);
        EXPECT_EQ(casterExpected.str(), casterResult.text);

        EXPECT_EQ(CLIENT_B.id, targetResult.connection.id);
        EXPECT_EQ(targetExpected.str(), targetResult.text);
    }


    TEST_F(MagicHandlerTestSuite, cannotCastConfuseOnOtherPlayerInDifferentRoom) {
        accountHandler.setRoomIdByClient(CLIENT_B, 42);
        ASSERT_NE(accountHandler.getRoomIdByClient(CLIENT_A), accountHandler.getRoomIdByClient(CLIENT_B));

        std::string spellName = CONFUSE_SPELL_NAME;
        std::string targetName = USERNAME_B;
        auto results = magicHandler.castSpell(CLIENT_A, spellName, targetName);

        ASSERT_EQ(1u, results.size());

        auto result = results.front();

        std::ostringstream casterExpected;
        casterExpected << "There is no player here with the name \"" << USERNAME_B << "\"\n";

        EXPECT_FALSE(magicHandler.isConfused(CLIENT_A));
        EXPECT_FALSE(magicHandler.isConfused(CLIENT_B));

        EXPECT_EQ(CLIENT_A.id, result.connection.id);
        EXPECT_EQ(casterExpected.str(), result.text);
    }


    TEST_F(MagicHandlerTestSuite, cannotCastConfuseOnSelfWhileConfused) {
        std::string spellName = CONFUSE_SPELL_NAME;
        std::string targetName = USERNAME_A;
        magicHandler.castSpell(CLIENT_A, spellName, targetName);

        ASSERT_TRUE(magicHandler.isConfused(CLIENT_A));
        ASSERT_FALSE(magicHandler.isConfused(CLIENT_B));

        auto results = magicHandler.castSpell(CLIENT_A, spellName, targetName);

        ASSERT_EQ(1u, results.size());

        auto result = results.front();

        std::ostringstream casterExpected;
        casterExpected << "You are already under the effects of the " << CONFUSE_SPELL_NAME << " spell!\n";

        EXPECT_EQ(CLIENT_A.id, result.connection.id);
        EXPECT_EQ(casterExpected.str(), result.text);
    }


    TEST_F(MagicHandlerTestSuite, cannotCastConfuseOnConfusedPlayer) {
        ASSERT_EQ(accountHandler.getRoomIdByClient(CLIENT_A), accountHandler.getRoomIdByClient(CLIENT_B));

        std::string spellName = CONFUSE_SPELL_NAME;
        std::string targetName = USERNAME_B;
        magicHandler.castSpell(CLIENT_A, spellName, targetName);

        ASSERT_FALSE(magicHandler.isConfused(CLIENT_A));
        ASSERT_TRUE(magicHandler.isConfused(CLIENT_B));

        auto results = magicHandler.castSpell(CLIENT_A, spellName, targetName);

        ASSERT_EQ(1u, results.size());

        auto casterResult = results.front();

        std::ostringstream casterExpected;
        casterExpected << targetName << " is already under the effects of the " << spellName << " spell!\n";

        ASSERT_EQ(CLIENT_A.id, casterResult.connection.id);
        ASSERT_EQ(casterExpected.str(), casterResult.text);
    }


    TEST_F(MagicHandlerTestSuite, canConvertMessageToPigLatin) {
        std::string message = "Hello, how are you?";
        magicHandler.confuseSpeech(message);

        std::ostringstream expected;
        expected << "elloHay, owhay areway ouyay?";

        EXPECT_EQ(expected.str(), message);
    }


    TEST_F(MagicHandlerTestSuite, canWaitUntilConfuseExpires) {
        std::string spellName = CONFUSE_SPELL_NAME;
        std::string targetName = USERNAME_A;
        magicHandler.castSpell(CLIENT_A, spellName, targetName);

        std::deque<Message> messages;
        for (int i = CONFUSE_DURATION; i >= 0; --i) {
            ASSERT_TRUE(magicHandler.isConfused(CLIENT_A));
            magicHandler.processCycle(messages);
        }

        EXPECT_FALSE(magicHandler.isConfused(CLIENT_A));
    }


    TEST_F(MagicHandlerTestSuite, canRemoveConfuseOnLogout) {
        std::string spellName = CONFUSE_SPELL_NAME;
        std::string targetName = USERNAME_A;
        magicHandler.castSpell(CLIENT_A, spellName, targetName);

        ASSERT_TRUE(magicHandler.isConfused(CLIENT_A));

        magicHandler.handleLogout(CLIENT_A);

        EXPECT_FALSE(magicHandler.isConfused(CLIENT_A));
    }


    TEST_F(MagicHandlerTestSuite, cannotCastBodySwapOnSelf) {
        std::string spellName = BODY_SWAP_SPELL_NAME;
        std::string targetName = USERNAME_A;
        auto results = magicHandler.castSpell(CLIENT_A, spellName, targetName);

        ASSERT_EQ(1u, results.size());

        auto result = results.back();

        std::ostringstream casterExpected;
        casterExpected << "You can't cast " << BODY_SWAP_SPELL_NAME << " on yourself!\n";

        EXPECT_EQ(CLIENT_A.id, result.connection.id);
        EXPECT_EQ(casterExpected.str(), result.text);
    }


    TEST_F(MagicHandlerTestSuite, canCastBodySwapOnOtherPlayerInSameRoom) {
        ASSERT_EQ(accountHandler.getRoomIdByClient(CLIENT_A), accountHandler.getRoomIdByClient(CLIENT_B));

        std::string spellName = BODY_SWAP_SPELL_NAME;
        std::string targetName = USERNAME_B;
        auto results = magicHandler.castSpell(CLIENT_A, spellName, targetName);

        ASSERT_EQ(2u, results.size());

        auto targetResult = results.front();
        auto casterResult = results.back();

        std::ostringstream casterExpected;
        casterExpected << "You have successfully swapped bodies with " << USERNAME_B << "\n";

        std::ostringstream targetExpected;
        targetExpected << USERNAME_A << " casts " << BODY_SWAP_SPELL_NAME << " on you!\n";

        EXPECT_EQ(CLIENT_A.id, casterResult.connection.id);
        EXPECT_EQ(casterExpected.str(), casterResult.text);

        EXPECT_EQ(CLIENT_B.id, targetResult.connection.id);
        EXPECT_EQ(targetExpected.str(), targetResult.text);

        EXPECT_EQ(CLIENT_A.id, accountHandler.getClientByUsername(USERNAME_B).id);
        EXPECT_EQ(CLIENT_B.id, accountHandler.getClientByUsername(USERNAME_A).id);
        EXPECT_TRUE(magicHandler.isBodySwapped(CLIENT_A));
        EXPECT_TRUE(magicHandler.isBodySwapped(CLIENT_B));
    }


    TEST_F(MagicHandlerTestSuite, cannotCastBodySwapOnOtherPlayerInDifferentRoom) {
        accountHandler.setRoomIdByClient(CLIENT_B, 42);
        ASSERT_NE(accountHandler.getRoomIdByClient(CLIENT_A), accountHandler.getRoomIdByClient(CLIENT_B));

        std::string spellName = BODY_SWAP_SPELL_NAME;
        std::string targetName = USERNAME_B;
        auto results = magicHandler.castSpell(CLIENT_A, spellName, targetName);

        ASSERT_EQ(1u, results.size());

        auto result = results.front();

        std::ostringstream casterExpected;
        casterExpected << "There is no one here with the name \"" << USERNAME_B << "\"\n";

        EXPECT_EQ(CLIENT_A.id, result.connection.id);
        EXPECT_EQ(casterExpected.str(), result.text);

        EXPECT_EQ(CLIENT_A.id, accountHandler.getClientByUsername(USERNAME_A).id);
        EXPECT_EQ(CLIENT_B.id, accountHandler.getClientByUsername(USERNAME_B).id);
        EXPECT_FALSE(magicHandler.isBodySwapped(CLIENT_A));
        EXPECT_FALSE(magicHandler.isBodySwapped(CLIENT_B));
    }


    TEST_F(MagicHandlerTestSuite, cannotCastBodySwapOnWhileSelfIsSwapped) {
        ASSERT_EQ(accountHandler.getRoomIdByClient(CLIENT_A), accountHandler.getRoomIdByClient(CLIENT_B));

        std::string spellName = BODY_SWAP_SPELL_NAME;
        std::string targetName = USERNAME_B;
        magicHandler.castSpell(CLIENT_A, spellName, targetName);

        ASSERT_EQ(CLIENT_A.id, accountHandler.getClientByUsername(USERNAME_B).id);
        ASSERT_EQ(CLIENT_B.id, accountHandler.getClientByUsername(USERNAME_A).id);
        ASSERT_TRUE(magicHandler.isBodySwapped(CLIENT_A));
        ASSERT_TRUE(magicHandler.isBodySwapped(CLIENT_B));

        auto results = magicHandler.castSpell(CLIENT_A, spellName, targetName);

        ASSERT_EQ(1u, results.size());

        auto casterResult = results.front();

        std::ostringstream casterExpected;
        casterExpected << "You can't cast " << BODY_SWAP_SPELL_NAME
                       << " while already under the effects of the spell!\n";

        EXPECT_EQ(CLIENT_A.id, casterResult.connection.id);
        EXPECT_EQ(casterExpected.str(), casterResult.text);

        EXPECT_EQ(CLIENT_A.id, accountHandler.getClientByUsername(USERNAME_B).id);
        EXPECT_EQ(CLIENT_B.id, accountHandler.getClientByUsername(USERNAME_A).id);
        EXPECT_TRUE(magicHandler.isBodySwapped(CLIENT_A));
        EXPECT_TRUE(magicHandler.isBodySwapped(CLIENT_B));
    }


    TEST_F(MagicHandlerTestSuite, cannotCastBodySwapOnWhileOtherPlayerIsSwapped) {
        // Register client C
        accountHandler.processRegistration(CLIENT_C);
        accountHandler.processRegistration(CLIENT_C, USERNAME_C);
        accountHandler.processRegistration(CLIENT_C, VALID_PASSWORD_STRING);
        accountHandler.processRegistration(CLIENT_C, VALID_PASSWORD_STRING);

        ASSERT_EQ(accountHandler.getRoomIdByClient(CLIENT_A), accountHandler.getRoomIdByClient(CLIENT_B));
        ASSERT_EQ(accountHandler.getRoomIdByClient(CLIENT_A), accountHandler.getRoomIdByClient(CLIENT_C));
        ASSERT_EQ(accountHandler.getRoomIdByClient(CLIENT_B), accountHandler.getRoomIdByClient(CLIENT_C));

        std::string spellName = BODY_SWAP_SPELL_NAME;
        std::string targetName = USERNAME_B;
        magicHandler.castSpell(CLIENT_A, spellName, targetName);

        ASSERT_EQ(CLIENT_A.id, accountHandler.getClientByUsername(USERNAME_B).id);
        ASSERT_EQ(CLIENT_B.id, accountHandler.getClientByUsername(USERNAME_A).id);
        ASSERT_EQ(CLIENT_C.id, accountHandler.getClientByUsername(USERNAME_C).id);
        ASSERT_TRUE(magicHandler.isBodySwapped(CLIENT_A));
        ASSERT_TRUE(magicHandler.isBodySwapped(CLIENT_B));
        ASSERT_FALSE(magicHandler.isBodySwapped(CLIENT_C));

        auto results = magicHandler.castSpell(CLIENT_C, spellName, targetName);

        ASSERT_EQ(1u, results.size());

        auto casterResult = results.front();

        std::ostringstream casterExpected;
        casterExpected << USERNAME_B << " is already under the effects of the " << BODY_SWAP_SPELL_NAME << " spell!\n";

        EXPECT_EQ(CLIENT_C.id, casterResult.connection.id);
        EXPECT_EQ(casterExpected.str(), casterResult.text);

        EXPECT_EQ(CLIENT_A.id, accountHandler.getClientByUsername(USERNAME_B).id);
        EXPECT_EQ(CLIENT_B.id, accountHandler.getClientByUsername(USERNAME_A).id);
        EXPECT_EQ(CLIENT_C.id, accountHandler.getClientByUsername(USERNAME_C).id);
        EXPECT_TRUE(magicHandler.isBodySwapped(CLIENT_A));
        EXPECT_TRUE(magicHandler.isBodySwapped(CLIENT_B));
        EXPECT_FALSE(magicHandler.isBodySwapped(CLIENT_C));
    }


    TEST_F(MagicHandlerTestSuite, canWaitUntilBodySwapExpires) {
        std::string spellName = BODY_SWAP_SPELL_NAME;
        std::string targetName = USERNAME_B;
        magicHandler.castSpell(CLIENT_A, spellName, targetName);

        std::deque<Message> messages;
        for (int i = BODY_SWAP_DURATION; i >= 0; --i) {
            ASSERT_EQ(CLIENT_A.id, accountHandler.getClientByUsername(USERNAME_B).id);
            ASSERT_EQ(CLIENT_B.id, accountHandler.getClientByUsername(USERNAME_A).id);
            ASSERT_TRUE(magicHandler.isBodySwapped(CLIENT_A));
            ASSERT_TRUE(magicHandler.isBodySwapped(CLIENT_B));
            magicHandler.processCycle(messages);
        }

        EXPECT_EQ(CLIENT_A.id, accountHandler.getClientByUsername(USERNAME_A).id);
        EXPECT_EQ(CLIENT_B.id, accountHandler.getClientByUsername(USERNAME_B).id);
        EXPECT_FALSE(magicHandler.isBodySwapped(CLIENT_A));
        EXPECT_FALSE(magicHandler.isBodySwapped(CLIENT_B));
    }


    TEST_F(MagicHandlerTestSuite, canRemoveBodySwapOnLogout) {
        std::string spellName = BODY_SWAP_SPELL_NAME;
        std::string targetName = USERNAME_B;
        magicHandler.castSpell(CLIENT_A, spellName, targetName);

        ASSERT_EQ(CLIENT_A.id, accountHandler.getClientByUsername(USERNAME_B).id);
        ASSERT_EQ(CLIENT_B.id, accountHandler.getClientByUsername(USERNAME_A).id);
        ASSERT_TRUE(magicHandler.isBodySwapped(CLIENT_A));
        ASSERT_TRUE(magicHandler.isBodySwapped(CLIENT_B));

        magicHandler.handleLogout(CLIENT_A);

        EXPECT_EQ(CLIENT_A.id, accountHandler.getClientByUsername(USERNAME_A).id);
        EXPECT_EQ(CLIENT_B.id, accountHandler.getClientByUsername(USERNAME_B).id);
        EXPECT_FALSE(magicHandler.isBodySwapped(CLIENT_A));
        EXPECT_FALSE(magicHandler.isBodySwapped(CLIENT_B));
    }

    TEST_F(MagicHandlerTestSuite, canGetSpellList) {
        auto result = magicHandler.getSpells();
        std::ostringstream expected;
        expected << "\n"
                 << "Spells:\n"
                 << "-------\n"
                 << "  - " << CONFUSE_SPELL_NAME << " (causes the target to temporarily speak in Pig Latin)\n"
                 << "  - " << BODY_SWAP_SPELL_NAME
                 << " (causes the caster to switch bodies with the target temporarily)\n";

        EXPECT_EQ(expected.str(), result);
    }
}
