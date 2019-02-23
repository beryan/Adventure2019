//
// Created by jnhkm on 2019-02-23.
//

#ifndef CHARACTERHANDLER_H
#define CHARACTERHANDLER_H

#include "Character.h"

using model::Character;

namespace action {
    class CharacterHandler {
    private:
        CharacterHandler();
    public:
        static void reduceHealth(Character &character, int change);

        static void addHealth(Character &character, int change);
    };
}


#endif //CHARACTERHANDLER_H
