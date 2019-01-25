//
// Created by jnhkm on 2019-01-24.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Character.h"

using model::Role;
using model::Character;

namespace {
    TEST(CharacterTestSuite, shouldReturnCorrectCharacterId) {
        int expected_id = 12345;
        Character character{expected_id};

        EXPECT_EQ(character.getId(), expected_id);
    }
}
