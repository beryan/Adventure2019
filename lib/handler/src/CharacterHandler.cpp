//
// Created by jnhkm on 2019-02-23.
//

#include "CharacterHandler.h"

using action::CharacterHandler;

namespace action {
    CharacterHandler::CharacterHandler() {};

    void CharacterHandler::reduceHealth(Character &character, int change) {
        if (logic.canReduceHealth(character.getHealth(), change)) {
            character.setHealth(character.getHealth() - change);
        }
        else {
            character.setHealth(logic.getMinHealth());
        }
    }

    void CharacterHandler::addHealth(Character &character, int change) {
        if (logic.canIncreaseHealth(character.getHealth(), change)) {
            character.setHealth(character.getHealth() + change);
        }
        else {
            character.setHealth(logic.getMaxHealth());
        }
    }
}