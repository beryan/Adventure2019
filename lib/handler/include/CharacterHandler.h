//
// Created by jnhkm on 2019-02-23.
//

#ifndef CHARACTERHANDLER_H
#define CHARACTERHANDLER_H

#include "Character.h"
#include "CharacterLogic.h"

using model::Character;
using logic::CharacterLogic;

namespace action {
    class CharacterHandler {
    private:
        CharacterLogic logic{};
    public:
        CharacterHandler();

        void reduceHealth(Character &character, int change);

        void addHealth(Character &character, int change);
    };
}


#endif //CHARACTERHANDLER_H
