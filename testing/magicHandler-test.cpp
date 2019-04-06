//
// Created by louis on 21/02/19.
//

#include "lib/gtest/gtest.h"
#include "lib/gmock/gmock.h"
#include "MagicHandler.h"
#include "WorldHandler.h"
#include "Server.h"

using handler::MagicHandler;
using handler::WorldHandler;
using networking::Connection;


constexpr Connection CLIENT_A = {100};
constexpr Connection CLIENT_B = {200};
constexpr Connection CLIENT_C = {300};

constexpr auto USERNAME_A = "Able";
constexpr auto USERNAME_B = "Baker";
constexpr auto USERNAME_C = "Charlie";

constexpr auto VALID_PASSWORD_STRING = "Valid Pass";

constexpr model::ID TEST_ROOM_ID = 1000;

constexpr model::ID NPC_A_ID = 100;
constexpr model::ID NPC_B_ID = 200;
constexpr auto NPC_A_KEYWORD = "atest";
constexpr auto NPC_B_KEYWORD = "btest";

constexpr auto CONFUSE_SPELL_NAME = "confuse";
constexpr auto BODY_SWAP_SPELL_NAME = "swap";
constexpr auto DECOY_SPELL_NAME = "decoy";
constexpr auto HEAL_SPELL_NAME = "heal";

constexpr unsigned int CONFUSE_DURATION = 50;
constexpr unsigned int BODY_SWAP_DURATION = 50;

constexpr auto CYCLES_PER_COMBAT_ROUND = 5;


namespace {
    class MagicHandlerTestSuite : public ::testing::Test {
    protected:
        AccountHandler accountHandler;
        WorldHandler worldHandler;
        CombatHandler combatHandler{accountHandler, worldHandler};
        MagicHandler magicHandler = MagicHandler{accountHandler, combatHandler};
        World world;

        virtual void SetUp() override {
            // Login client A
            accountHandler.processLogin(CLIENT_A);
            accountHandler.processLogin(CLIENT_A, USERNAME_A);
            accountHandler.processLogin(CLIENT_A, VALID_PASSWORD_STRING);
            accountHandler.setRoomIdByClient(CLIENT_A, TEST_ROOM_ID);

            // Login client B
            accountHandler.processLogin(CLIENT_B);
            accountHandler.processLogin(CLIENT_B, USERNAME_B);
            accountHandler.processLogin(CLIENT_B, VALID_PASSWORD_STRING);
            accountHandler.setRoomIdByClient(CLIENT_B, TEST_ROOM_ID);

            // Construct World
            Room room = {TEST_ROOM_ID, "Test room 1", {"Test room 1 description"}};
            NPC npcA{
                NPC_A_ID,
                {NPC_A_KEYWORD},
                {"Long description."},
                "Test NPC 1",
                {"Interaction text."}
            };
            std::ostringstream uniqueIdA;
            uniqueIdA << "1" << std::to_string(NPC_A_ID) << std::to_string(TEST_ROOM_ID);
            npcA.setUniqueId(std::stoi(uniqueIdA.str()));

            NPC npcB{
                NPC_B_ID,
                {NPC_B_KEYWORD},
                {"Long description."},
                "Test NPC 1",
                {"Interaction text."}
            };
            std::ostringstream uniqueIdB;
            uniqueIdB << "1" << std::to_string(NPC_B_ID) << std::to_string(TEST_ROOM_ID);
            npcB.setUniqueId(std::stoi(uniqueIdB.str()));

            room.addNPC(npcA);
            room.addNPC(npcB);

            Area area = Area("Testing area");
            area.addRoom(room);
            world.addArea(area);
            worldHandler.setWorld(world);
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
     * 18. A player can cast Heal on themselves
     * 19. A player can cast Heal on another player in the same room
     * 20. A player cannot cast Heal on another player in a different room
     * 21. A player cannot cast Heal while in combat
     * 22. A player caanot cast Heal on target while in combat
     * 23. A player cannot cast Heal while target is in combat
     * 24. A player cannot cast Decoy while not in combat
     * 25. A player can cast Decoy while in combat
     * 26. A player cannot cast Decoy if they already have an active decoy
     * 27. Can get spells list
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
        casterExpected << "There are no spells with the name of \"" << spellName << "\".\n";

        EXPECT_EQ(CLIENT_A.id, result.connection.id);
        EXPECT_EQ(casterExpected.str(), result.text);
    }


    TEST_F(MagicHandlerTestSuite, canCastConfuseOnSelf) {
        std::string targetName = USERNAME_A;
        auto results = magicHandler.castSpell(CLIENT_A, CONFUSE_SPELL_NAME, targetName);

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

        std::string targetName = USERNAME_B;
        auto results = magicHandler.castSpell(CLIENT_A, CONFUSE_SPELL_NAME, targetName);

        ASSERT_EQ(2u, results.size());

        auto casterResult = results.front();
        auto targetResult = results.back();

        std::ostringstream casterExpected;
        casterExpected << "You cast " << CONFUSE_SPELL_NAME << " on " << USERNAME_B << ".\n";

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

        std::string targetName = USERNAME_B;
        auto results = magicHandler.castSpell(CLIENT_A, CONFUSE_SPELL_NAME, targetName);

        ASSERT_EQ(1u, results.size());

        auto result = results.front();

        std::ostringstream casterExpected;
        casterExpected << "There is no player here with the name \"" << USERNAME_B << "\".\n";

        EXPECT_FALSE(magicHandler.isConfused(CLIENT_A));
        EXPECT_FALSE(magicHandler.isConfused(CLIENT_B));

        EXPECT_EQ(CLIENT_A.id, result.connection.id);
        EXPECT_EQ(casterExpected.str(), result.text);
    }


    TEST_F(MagicHandlerTestSuite, cannotCastConfuseOnSelfWhileConfused) {
        std::string targetName = USERNAME_A;
        magicHandler.castSpell(CLIENT_A, CONFUSE_SPELL_NAME, targetName);

        ASSERT_TRUE(magicHandler.isConfused(CLIENT_A));
        ASSERT_FALSE(magicHandler.isConfused(CLIENT_B));

        auto results = magicHandler.castSpell(CLIENT_A, CONFUSE_SPELL_NAME, targetName);

        ASSERT_EQ(1u, results.size());

        auto result = results.front();

        std::ostringstream casterExpected;
        casterExpected << "You are already under the effects of the " << CONFUSE_SPELL_NAME << " spell!\n";

        EXPECT_EQ(CLIENT_A.id, result.connection.id);
        EXPECT_EQ(casterExpected.str(), result.text);
    }


    TEST_F(MagicHandlerTestSuite, cannotCastConfuseOnConfusedPlayer) {
        ASSERT_EQ(accountHandler.getRoomIdByClient(CLIENT_A), accountHandler.getRoomIdByClient(CLIENT_B));

        std::string targetName = USERNAME_B;
        magicHandler.castSpell(CLIENT_A, CONFUSE_SPELL_NAME, targetName);

        ASSERT_FALSE(magicHandler.isConfused(CLIENT_A));
        ASSERT_TRUE(magicHandler.isConfused(CLIENT_B));

        auto results = magicHandler.castSpell(CLIENT_A, CONFUSE_SPELL_NAME, targetName);

        ASSERT_EQ(1u, results.size());

        auto casterResult = results.front();

        std::ostringstream casterExpected;
        casterExpected << targetName << " is already under the effects of the " << CONFUSE_SPELL_NAME << " spell!\n";

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
        std::string targetName = USERNAME_A;
        magicHandler.castSpell(CLIENT_A, CONFUSE_SPELL_NAME, targetName);

        std::deque<Message> messages;
        for (int i = CONFUSE_DURATION; i >= 0; --i) {
            ASSERT_TRUE(magicHandler.isConfused(CLIENT_A));
            magicHandler.processCycle(messages);
        }

        EXPECT_FALSE(magicHandler.isConfused(CLIENT_A));
    }


    TEST_F(MagicHandlerTestSuite, canRemoveConfuseOnLogout) {
        std::string targetName = USERNAME_A;
        magicHandler.castSpell(CLIENT_A, CONFUSE_SPELL_NAME, targetName);

        ASSERT_TRUE(magicHandler.isConfused(CLIENT_A));

        magicHandler.handleLogout(CLIENT_A);

        EXPECT_FALSE(magicHandler.isConfused(CLIENT_A));
    }


    TEST_F(MagicHandlerTestSuite, cannotCastBodySwapOnSelf) {
        std::string targetName = USERNAME_A;
        auto results = magicHandler.castSpell(CLIENT_A, BODY_SWAP_SPELL_NAME, targetName);

        ASSERT_EQ(1u, results.size());

        auto result = results.back();

        std::ostringstream casterExpected;
        casterExpected << "You can't cast " << BODY_SWAP_SPELL_NAME << " on yourself!\n";

        EXPECT_EQ(CLIENT_A.id, result.connection.id);
        EXPECT_EQ(casterExpected.str(), result.text);
    }


    TEST_F(MagicHandlerTestSuite, canCastBodySwapOnOtherPlayerInSameRoom) {
        ASSERT_EQ(accountHandler.getRoomIdByClient(CLIENT_A), accountHandler.getRoomIdByClient(CLIENT_B));

        std::string targetName = USERNAME_B;
        auto results = magicHandler.castSpell(CLIENT_A, BODY_SWAP_SPELL_NAME, targetName);

        ASSERT_EQ(2u, results.size());

        auto targetResult = results.front();
        auto casterResult = results.back();

        std::ostringstream casterExpected;
        casterExpected << "You have successfully swapped bodies with " << USERNAME_B << ".\n";

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

        std::string targetName = USERNAME_B;
        auto results = magicHandler.castSpell(CLIENT_A, BODY_SWAP_SPELL_NAME, targetName);

        ASSERT_EQ(1u, results.size());

        auto result = results.front();

        std::ostringstream casterExpected;
        casterExpected << "There is no one here with the name \"" << USERNAME_B << "\".\n";

        EXPECT_EQ(CLIENT_A.id, result.connection.id);
        EXPECT_EQ(casterExpected.str(), result.text);

        EXPECT_EQ(CLIENT_A.id, accountHandler.getClientByUsername(USERNAME_A).id);
        EXPECT_EQ(CLIENT_B.id, accountHandler.getClientByUsername(USERNAME_B).id);
        EXPECT_FALSE(magicHandler.isBodySwapped(CLIENT_A));
        EXPECT_FALSE(magicHandler.isBodySwapped(CLIENT_B));
    }


    TEST_F(MagicHandlerTestSuite, cannotCastBodySwapOnWhileSelfIsSwapped) {
        ASSERT_EQ(accountHandler.getRoomIdByClient(CLIENT_A), accountHandler.getRoomIdByClient(CLIENT_B));

        std::string targetName = USERNAME_B;
        magicHandler.castSpell(CLIENT_A, BODY_SWAP_SPELL_NAME, targetName);

        ASSERT_EQ(CLIENT_A.id, accountHandler.getClientByUsername(USERNAME_B).id);
        ASSERT_EQ(CLIENT_B.id, accountHandler.getClientByUsername(USERNAME_A).id);
        ASSERT_TRUE(magicHandler.isBodySwapped(CLIENT_A));
        ASSERT_TRUE(magicHandler.isBodySwapped(CLIENT_B));

        auto results = magicHandler.castSpell(CLIENT_A, BODY_SWAP_SPELL_NAME, targetName);

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
        // Login client C
        accountHandler.processLogin(CLIENT_C);
        accountHandler.processLogin(CLIENT_C, USERNAME_C);
        accountHandler.processLogin(CLIENT_C, VALID_PASSWORD_STRING);
        accountHandler.setRoomIdByClient(CLIENT_C, TEST_ROOM_ID);

        ASSERT_EQ(accountHandler.getRoomIdByClient(CLIENT_A), accountHandler.getRoomIdByClient(CLIENT_B));
        ASSERT_EQ(accountHandler.getRoomIdByClient(CLIENT_A), accountHandler.getRoomIdByClient(CLIENT_C));
        ASSERT_EQ(accountHandler.getRoomIdByClient(CLIENT_B), accountHandler.getRoomIdByClient(CLIENT_C));

        std::string targetName = USERNAME_B;
        magicHandler.castSpell(CLIENT_A, BODY_SWAP_SPELL_NAME, targetName);

        ASSERT_EQ(CLIENT_A.id, accountHandler.getClientByUsername(USERNAME_B).id);
        ASSERT_EQ(CLIENT_B.id, accountHandler.getClientByUsername(USERNAME_A).id);
        ASSERT_EQ(CLIENT_C.id, accountHandler.getClientByUsername(USERNAME_C).id);
        ASSERT_TRUE(magicHandler.isBodySwapped(CLIENT_A));
        ASSERT_TRUE(magicHandler.isBodySwapped(CLIENT_B));
        ASSERT_FALSE(magicHandler.isBodySwapped(CLIENT_C));

        auto results = magicHandler.castSpell(CLIENT_C, BODY_SWAP_SPELL_NAME, targetName);

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
        std::string targetName = USERNAME_B;
        magicHandler.castSpell(CLIENT_A, BODY_SWAP_SPELL_NAME, targetName);

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
        std::string targetName = USERNAME_B;
        magicHandler.castSpell(CLIENT_A, BODY_SWAP_SPELL_NAME, targetName);

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


    TEST_F(MagicHandlerTestSuite, canCastHealOnSelf) {
        std::string targetName = USERNAME_A;
        auto &player = accountHandler.getPlayerByClient(CLIENT_A);

        // With own username as target parameter
        player.setHealth(1);
        auto results = magicHandler.castSpell(CLIENT_A, HEAL_SPELL_NAME, targetName);

        ASSERT_EQ(1u, results.size());

        auto result = results.back();

        std::ostringstream casterExpected;
        casterExpected << "You cast " << HEAL_SPELL_NAME << " on yourself.\n";

        EXPECT_EQ(CLIENT_A.id, result.connection.id);
        EXPECT_EQ(Character::STARTING_HEALTH, player.getHealth());
        EXPECT_EQ(casterExpected.str(), result.text);

        // With no username provided in target parameter
        player.setHealth(1);
        results = magicHandler.castSpell(CLIENT_A, HEAL_SPELL_NAME);

        ASSERT_EQ(1u, results.size());

        result = results.back();

        EXPECT_EQ(CLIENT_A.id, result.connection.id);
        EXPECT_EQ(Character::STARTING_HEALTH, player.getHealth());
        EXPECT_EQ(casterExpected.str(), result.text);
    }


    TEST_F(MagicHandlerTestSuite, canCastHealOnOtherPlayerInSameRoom) {
        ASSERT_EQ(accountHandler.getRoomIdByClient(CLIENT_A), accountHandler.getRoomIdByClient(CLIENT_B));

        std::string targetName = USERNAME_B;
        auto &targetPlayer = accountHandler.getPlayerByClient(CLIENT_B);
        targetPlayer.setHealth(1);
        auto results = magicHandler.castSpell(CLIENT_A, HEAL_SPELL_NAME, targetName);

        ASSERT_EQ(2u, results.size());

        auto casterResult = results.front();
        auto targetResult = results.back();

        std::ostringstream casterExpected;
        casterExpected << "You cast " << HEAL_SPELL_NAME << " on " << USERNAME_B << ".\n";

        std::ostringstream targetExpected;
        targetExpected << USERNAME_A << " casts " << HEAL_SPELL_NAME << " on you!" << "\n";

        EXPECT_EQ(CLIENT_A.id, casterResult.connection.id);
        EXPECT_EQ(casterExpected.str(), casterResult.text);

        EXPECT_EQ(CLIENT_B.id, targetResult.connection.id);
        EXPECT_EQ(Character::STARTING_HEALTH, targetPlayer.getHealth());
        EXPECT_EQ(targetExpected.str(), targetResult.text);
    }


    TEST_F(MagicHandlerTestSuite, cannotCastHealOnOtherPlayerInDifferentRoom) {
        accountHandler.setRoomIdByClient(CLIENT_B, 42);
        ASSERT_NE(accountHandler.getRoomIdByClient(CLIENT_A), accountHandler.getRoomIdByClient(CLIENT_B));

        std::string targetName = USERNAME_B;
        auto &targetPlayer = accountHandler.getPlayerByClient(CLIENT_B);

        targetPlayer.setHealth(1);
        auto results = magicHandler.castSpell(CLIENT_A, HEAL_SPELL_NAME, targetName);

        ASSERT_EQ(1u, results.size());

        auto result = results.front();

        std::ostringstream casterExpected;
        casterExpected << "There is no player here with the name \"" << USERNAME_B << "\".\n";

        EXPECT_EQ(CLIENT_A.id, result.connection.id);
        EXPECT_EQ(1, targetPlayer.getHealth());
        EXPECT_EQ(casterExpected.str(), result.text);
    }


    TEST_F(MagicHandlerTestSuite, cannotCastHealOnSelfWhileInCombat) {
        auto &player = accountHandler.getPlayerByClient(CLIENT_A);
        auto &npc = worldHandler.findRoom(TEST_ROOM_ID).getNpcByKeyword(NPC_A_KEYWORD);

        ASSERT_FALSE(combatHandler.isInCombat(player.getId()));
        ASSERT_EQ(Character::STARTING_HEALTH, player.getHealth());

        player.setHealth(90);
        combatHandler.attack(CLIENT_A, NPC_A_KEYWORD);
        auto healthAfter = player.getHealth();

        ASSERT_TRUE(combatHandler.areInCombat(player.getId(), npc.getUniqueId()));

        auto results = magicHandler.castSpell(CLIENT_A, HEAL_SPELL_NAME);

        ASSERT_EQ(1u, results.size());

        auto result = results.front();

        std::ostringstream casterExpected;
        casterExpected << "You can't cast " << HEAL_SPELL_NAME << " while in combat.\n";

        EXPECT_EQ(CLIENT_A.id, result.connection.id);
        EXPECT_LT(healthAfter, Character::STARTING_HEALTH);
        EXPECT_EQ(casterExpected.str(), result.text);
    }


    TEST_F(MagicHandlerTestSuite, cannotCastHealOnOtherPlayerWhileInCombat) {
        auto &fightingPlayer = accountHandler.getPlayerByClient(CLIENT_A);
        auto &targetPlayer = accountHandler.getPlayerByClient(CLIENT_B);
        auto &npc = worldHandler.findRoom(TEST_ROOM_ID).getNpcByKeyword(NPC_A_KEYWORD);

        ASSERT_EQ(Character::STARTING_HEALTH, targetPlayer.getHealth());
        targetPlayer.setHealth(90);
        auto healthAfter = targetPlayer.getHealth();

        combatHandler.attack(CLIENT_A, NPC_A_KEYWORD);
        ASSERT_TRUE(combatHandler.areInCombat(fightingPlayer.getId(), npc.getUniqueId()));

        auto results = magicHandler.castSpell(CLIENT_A, HEAL_SPELL_NAME, USERNAME_B);

        ASSERT_EQ(1u, results.size());

        auto result = results.front();

        std::ostringstream casterExpected;
        casterExpected << "You can't cast " << HEAL_SPELL_NAME << " while in combat.\n";

        EXPECT_EQ(CLIENT_A.id, result.connection.id);
        EXPECT_LT(healthAfter, Character::STARTING_HEALTH);
        EXPECT_EQ(casterExpected.str(), result.text);
    }


    TEST_F(MagicHandlerTestSuite, cannotCastHealOnPlayerInCombat) {
        ASSERT_EQ(accountHandler.getRoomIdByClient(CLIENT_A), accountHandler.getRoomIdByClient(CLIENT_B));

        auto &fightingPlayer = accountHandler.getPlayerByClient(CLIENT_B);
        auto &npc = worldHandler.findRoom(TEST_ROOM_ID).getNpcByKeyword(NPC_A_KEYWORD);

        ASSERT_EQ(Character::STARTING_HEALTH, fightingPlayer.getHealth());

        fightingPlayer.setHealth(90);
        combatHandler.attack(CLIENT_B, NPC_A_KEYWORD);
        auto healthAfter = fightingPlayer.getHealth();

        ASSERT_TRUE(combatHandler.areInCombat(fightingPlayer.getId(), npc.getUniqueId()));

        auto results = magicHandler.castSpell(CLIENT_A, HEAL_SPELL_NAME, USERNAME_B);

        ASSERT_EQ(1u, results.size());

        auto result = results.front();

        std::ostringstream expected;
        expected << "You can't cast " << HEAL_SPELL_NAME << " on "
                 << USERNAME_B << " while they are in combat.\n";

        EXPECT_EQ(CLIENT_A.id, result.connection.id);
        EXPECT_LT(healthAfter, Character::STARTING_HEALTH);
        EXPECT_EQ(expected.str(), result.text);
    }


    TEST_F(MagicHandlerTestSuite, cannotCastDecoyWhileNotInCombat) {
        auto playerId = accountHandler.getPlayerIdByClient(CLIENT_A);

        ASSERT_NO_THROW(worldHandler.findRoom(TEST_ROOM_ID).getNpcByKeyword(NPC_A_KEYWORD));
        ASSERT_FALSE(combatHandler.isInCombat(playerId));

        auto results = magicHandler.castSpell(CLIENT_A, DECOY_SPELL_NAME, NPC_A_KEYWORD);

        ASSERT_EQ(1u, results.size());

        auto result = results.front();

        std::ostringstream expected;
        expected << "You can only cast " << DECOY_SPELL_NAME << " while in combat.\n";

        EXPECT_EQ(CLIENT_A.id, result.connection.id);
        EXPECT_EQ(expected.str(), result.text);
    }


    TEST_F(MagicHandlerTestSuite, canCastDecoyWhileInCombat) {
        auto playerId = accountHandler.getPlayerIdByClient(CLIENT_A);
        auto &npc = worldHandler.findRoom(TEST_ROOM_ID).getNpcByKeyword(NPC_A_KEYWORD);

        ASSERT_FALSE(combatHandler.isInCombat(playerId));
        ASSERT_FALSE(combatHandler.isInCombat(npc.getUniqueId()));

        combatHandler.attack(CLIENT_A, NPC_A_KEYWORD);

        ASSERT_TRUE(combatHandler.areInCombat(playerId, npc.getUniqueId()));

        auto results = magicHandler.castSpell(CLIENT_A, DECOY_SPELL_NAME);

        ASSERT_EQ(1u, results.size());

        auto result = results.front();

        std::ostringstream expected;
        expected << "You create a decoy of yourself and escape from combat.\n";

        EXPECT_EQ(CLIENT_A.id, result.connection.id);
        EXPECT_EQ(expected.str(), result.text);
        EXPECT_FALSE(combatHandler.isInCombat(playerId));
    }


    TEST_F(MagicHandlerTestSuite, cannotCastDecoyWhileDecoyAlreadyExists) {
        auto playerId = accountHandler.getPlayerIdByClient(CLIENT_A);
        auto npcA = worldHandler.findRoom(TEST_ROOM_ID).getNpcByKeyword(NPC_A_KEYWORD);
        auto npcB = worldHandler.findRoom(TEST_ROOM_ID).getNpcByKeyword(NPC_B_KEYWORD);

        ASSERT_FALSE(combatHandler.isInCombat(playerId));
        ASSERT_FALSE(combatHandler.isInCombat(npcA.getUniqueId()));

        combatHandler.attack(CLIENT_A, NPC_A_KEYWORD);
        ASSERT_TRUE(combatHandler.areInCombat(playerId, npcA.getUniqueId()));

        magicHandler.castSpell(CLIENT_A, DECOY_SPELL_NAME);
        ASSERT_FALSE(combatHandler.isInCombat(playerId));

        combatHandler.attack(CLIENT_A, NPC_B_KEYWORD);
        ASSERT_TRUE(combatHandler.areInCombat(playerId, npcB.getUniqueId()));

        auto results = magicHandler.castSpell(CLIENT_A, DECOY_SPELL_NAME);
        ASSERT_EQ(1u, results.size());

        auto result = results.front();

        std::ostringstream expected;
        expected << "You can only have one active decoy at a time!\n";

        ASSERT_TRUE(combatHandler.areInCombat(playerId, npcB.getUniqueId()));
        EXPECT_EQ(CLIENT_A.id, result.connection.id);
        EXPECT_EQ(expected.str(), result.text);
    }


    TEST_F(MagicHandlerTestSuite, canGetSpellList) {
        auto result = magicHandler.getSpells();
        std::ostringstream expected;
        expected << "\n"
                 << "Spells:\n"
                 << "-------\n"
                 << "  - " << CONFUSE_SPELL_NAME
                 << " (causes the target player to temporarily speak in Pig Latin)\n"

                 << "  - " << BODY_SWAP_SPELL_NAME
                 << " (causes the caster to switch bodies with the target player temporarily)\n"

                 << "  - " << DECOY_SPELL_NAME
                 << " (creates a decoy of yourself in combat to escape)\n"

                 << "  - " << HEAL_SPELL_NAME
                 << " (fully restores target player's health. Can not be used if caster or target is in combat)\n";

        EXPECT_EQ(expected.str(), result);
    }
}
