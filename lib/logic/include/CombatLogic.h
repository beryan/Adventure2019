//
// Created by jnhkm on 2019-03-10.
//

#ifndef WEBSOCKETNETWORKING_COMBATLOGIC_H
#define WEBSOCKETNETWORKING_COMBATLOGIC_H

#include "Character.h"
#include "CharacterLogic.h"

namespace logic {
    class CombatLogic {
    private:
    public:
        CombatLogic();

        bool canAttackTarget(const Character &attacker, const Character &defender);

        bool canFlee(const Character &character);
    };
};



#endif //WEBSOCKETNETWORKING_COMBATLOGIC_H
