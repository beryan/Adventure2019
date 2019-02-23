//
// Created by jnhkm on 2019-01-24.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "CharacterHandler.h"

using model::Role;
using action::CharacterHandler;

namespace {
    TEST(CharacterTestSuite, canConstructCharacter) {
        int expected_id = 12345;
        Character character{expected_id};

        EXPECT_EQ(character.getId(), expected_id);
    }

    TEST(CharacterTestSuite, canReturnCorrectHealthAfterReducing) {
        int damage = 30.0;
        int expected_id = 12345;
        int expected_health = Character::STARTING_HEALTH - damage;

        Character character{expected_id};
        CharacterHandler::reduceHealth(character, damage);

        EXPECT_EQ(character.getHealth(), expected_health);
    }

    TEST(CharacterTestSuite, canReturnCorrectHealthAfterIncreasingWithDamagedHealth) {
        int heal = 30.0;
        int expected_id = 12345;
        int expected_health = 80;

        Character character{expected_id};
        character.setHealth(50);
        CharacterHandler::addHealth(character, heal);

        EXPECT_EQ(character.getHealth(), expected_health);
    }

    TEST(CharacterTestSuite, canReturnCorrectHealthAfterIncreasingWithFullHealth) {
        int heal = 30.0;
        int expected_id = 12345;
        int expected_health = Character::STARTING_HEALTH + heal;

        Character character{expected_id};
        CharacterHandler::addHealth(character, heal);

        EXPECT_EQ(character.getHealth(), Character::STARTING_HEALTH);
        EXPECT_NE(character.getHealth(), expected_health);
    }
}
