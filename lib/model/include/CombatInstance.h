//
// Created by jnhkm on 2019-03-12.
//

#ifndef COMBATINSTANCE_H
#define COMBATINSTANCE_H

namespace model {
    constexpr unsigned int CYCLES_PER_ROUND = 7;

    struct CombatInstance {
        int attackerID;
        int defenderID;
        unsigned int roundCyclesRemaining;

        CombatInstance(int attackerID, int defenderID) {
            this->attackerID = attackerID;
            this->defenderID = defenderID;
            this->roundCyclesRemaining = CYCLES_PER_ROUND;
        }

        void decrement() {
            --this->roundCyclesRemaining;
        }

        void endRound() {
            this->roundCyclesRemaining = CYCLES_PER_ROUND;
        }

        bool operator==(const CombatInstance& other) const {
            return (this->attackerID == other.attackerID && this->defenderID == other.defenderID) ||
                   (this->defenderID == other.attackerID && this->attackerID == other.defenderID);
        }
    };
}

#endif //COMBATINSTANCE_H
