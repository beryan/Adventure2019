//
// Created by louis on 18/03/19.
//

#ifndef AVATAR_H
#define AVATAR_H

#include <string>

namespace model {
    struct Avatar {
        std::string gender;
        std::string race;
        std::string trait1;
        std::string trait2;

        bool isDefined() {
            return !(this->gender.empty() || this->race.empty() || this->trait1.empty() || this->trait2.empty());
        }
    };
}

#endif //AVATAR_H
