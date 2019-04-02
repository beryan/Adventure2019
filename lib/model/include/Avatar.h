//
// Created by louis on 18/03/19.
//

#ifndef AVATAR_H
#define AVATAR_H

#include <string>
#include "json.hpp"

using json = nlohmann::json;

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

    inline void from_json(const json &j, Avatar &a) {
        std::string gender = j.at("gender").get<std::string>();
        std::string race = j.at("race").get<std::string>();

        if (gender == "m" || gender == "male") {
            a.gender = model::Gender::Male;

        } else if (gender == "f" || gender == "female") {
            a.gender = model::Gender::Female;

        }

        a.race = Avatar::stringToRaceMap.at(race);
        a.trait1 = j.at("trait1").get<std::string>();
        a.trait2 = j.at("trait2").get<std::string>();
    }

    inline void to_json(json& j, const Avatar &a){
        j = {
                {"gender", Avatar::genderToStringMap.at(a.gender)},
                {"race", Avatar::raceToStringMap.at(a.race)},
                {"trait1", a.trait1},
                {"trait2", a.trait2}
        };
    }
}

#endif //AVATAR_H
