//
// Created by louis on 18/03/19.
//

#ifndef AVATAR_H
#define AVATAR_H

#include <string>

namespace model {
    enum class Gender{Male, Female, Unset};
    enum class Race{Dwarf, Elf, Human, Orc, Unset};

    struct Avatar {
        Gender gender = Gender::Unset;
        Race race = Race::Unset;
        std::string trait1;
        std::string trait2;

        static const inline std::map<Gender, std::string> genderToStringMap = {
                {Gender::Female, "female"},
                {Gender::Male, "male"}
        };

        static const inline std::map<std::string, model::Race> stringToRaceMap = {
                {"dwarf", Race::Dwarf},
                {"elf", Race::Elf},
                {"human", Race::Human},
                {"orc", Race::Orc}
        };

        static const inline std::map<model::Race, std::string> raceToStringMap = {
                {Race::Dwarf, "dwarf"},
                {Race::Elf, "elf"},
                {Race::Human, "human"},
                {Race::Orc, "orc"}
        };

        bool isDefined() const {
            return !(this->gender == Gender::Unset ||
                     this->race == Race::Unset ||
                     this->trait1.empty() ||
                     this->trait2.empty());
        }
    };
}

#endif //AVATAR_H
