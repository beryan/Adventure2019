//
// Created by jnhkm on 2019-01-24.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Character.h"

using model::Role;
using model::Character;

namespace {
    TEST(CharacterTestSuite, canConstructCharacter) {
        int expected_id = 12345;
        Character character{expected_id};

        EXPECT_EQ(character.getId(), expected_id);
    }

    TEST(CharacterTestSuite, canReturnCorrectHealthAfterReducing) {
        int damage = 30.0;
        int expected_id = 12345;
        double expected_health = Character::STARTING_HEALTH - damage;

        Character character{expected_id};
        character.reduceHealth(damage);

        EXPECT_EQ(character.getHealth(), expected_health);
    }
}
