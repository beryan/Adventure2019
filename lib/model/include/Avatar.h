//
// Created by louis on 18/03/19.
//

#ifndef AVATAR_H
#define AVATAR_H

#include <string>
#include "json.hpp"

using Json = nlohmann::json;

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

    inline void from_json(const Json &avatarJson, Avatar &avatar) {
        std::string gender = avatarJson.at("gender").get<std::string>();
        std::string race = avatarJson.at("race").get<std::string>();

        if (gender == "m" || gender == "male") {
            avatar.gender = model::Gender::Male;

        } else if (gender == "f" || gender == "female") {
            avatar.gender = model::Gender::Female;

        }

        avatar.race = Avatar::stringToRaceMap.at(race);
        avatar.trait1 = avatarJson.at("trait1").get<std::string>();
        avatar.trait2 = avatarJson.at("trait2").get<std::string>();
    }

    inline void to_json(Json& avatarJson, const Avatar &avatar){
        avatarJson = {
                {"gender", Avatar::genderToStringMap.at(avatar.gender)},
                {"race", Avatar::raceToStringMap.at(avatar.race)},
                {"trait1", avatar.trait1},
                {"trait2", avatar.trait2}
        };
    }
}

#endif //AVATAR_H
