//
// Created by jnhkm on 2019-03-10.
//

#ifndef WEBSOCKETNETWORKING_COMBATLOGIC_H
#define WEBSOCKETNETWORKING_COMBATLOGIC_H

#include "Character.h"

using model::Character;

namespace logic {
    struct CombatState {
        int attackerID;
        int defenderID;

        CombatState(int attackerID, int defenderID) {
            this->attackerID = attackerID;
            this->defenderID = defenderID;
        }

        bool operator==(const CombatState& other) const {
            return (this->attackerID == other.attackerID && this->defenderID == other.defenderID) ||
                    (this->defenderID == other.attackerID && this->attackerID == other.defenderID);
        }
    };

    constexpr int DEFAULT_MAX_HEALTH = 100;
    constexpr int DEFAULT_MIN_HEALTH = 0;

    class CombatLogic {
    private:
        std::vector<CombatState> active_combats;
    public:
        CombatLogic(std::vector<CombatState> &active_combats);

        bool canEnterCombat(const CombatState &combat);

        bool canExitCombat(const CombatState &combat);

        bool canAttackTarget(const Character &attacker, const Character &defender);

        bool canHealTarget(const Character &healer, const Character &target);

        bool canFlee(const Character &character);
    };
};



#endif //WEBSOCKETNETWORKING_COMBATLOGIC_H
