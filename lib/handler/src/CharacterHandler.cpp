//
// Created by jnhkm on 2019-02-23.
//

#include "CharacterHandler.h"
#include "CharacterLogic.h"

using action::CharacterHandler;
using logic::CharacterLogic;

namespace action {
    void CharacterHandler::reduceHealth(Character &character, int change) {
        if (CharacterLogic::canReduceHealth(character.getHealth(), change)) {
            character.setHealth(character.getHealth() - change);
        }
        else {
            character.setHealth(0);
        }
    }

    void CharacterHandler::addHealth(Character &character, int change) {
        if (CharacterLogic::canIncreaseHealth(character.getHealth(), change)) {
            character.setHealth(character.getHealth() + change);
        }
        else {
            character.setHealth(Character::MAX_HEALTH);
        }
    }
}