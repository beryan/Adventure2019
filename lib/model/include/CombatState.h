//
// Created by jnhkm on 2019-03-12.
//

#ifndef COMBATSTATE_H
#define COMBATSTATE_H

namespace model {
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
}

#endif //COMBATSTATE_H
