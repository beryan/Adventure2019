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
        CharacterLogic logic = {};
    public:
        CombatLogic();

        bool canAttackTarget(Character attacker, Character defender);

        bool canFlee(Character character);
    };
};



#endif //WEBSOCKETNETWORKING_COMBATLOGIC_H
