//
// Created by louis on 18/03/19.
//

#include <sstream>
#include <boost/algorithm/string/case_conv.hpp>
#include "Player.h"
#include "AvatarHandler.h"
#include "DataManager.h"

using handler::AvatarHandler;

namespace handler {
    AvatarHandler::AvatarHandler(AccountHandler &accountHandler):
        accountHandler(accountHandler){}


    std::string
    AvatarHandler::processGender(const Connection &client, const std::string &input) {
        model::Gender selectedGender;
        std::ostringstream message;

        if (input == "m" || input == "male") {
            selectedGender = model::Gender::Male;

        } else if (input == "f" || input == "female") {
            selectedGender = model::Gender::Female;

        } else {
            message << "Invalid input, please try again.\n"
                    << "Enter the gender of your character: [Male/Female]\n";
            return message.str();
        }

        this->avatarsInCreation.at(client).gender = selectedGender;
        this->creatingClients.at(client) = AvatarCreationStage::Race;

        message << Avatar::genderToStringMap.at(selectedGender) << "\n"
                << "Enter the race of your character: [Human/Dwarf/Elf/Orc]\n";
        return message.str();
    }


    std::string
    AvatarHandler::processRace(const Connection &client, const std::string &input) {
        std::ostringstream message;
        if (!static_cast<bool>(Avatar::stringToRaceMap.count(input))) {
            message << "Invalid input, please try again.\n"
                    << "Enter the race of your character: [Human/Dwarf/Elf/Orc]\n";
            return message.str();
        }

        auto selectedRace = Avatar::stringToRaceMap.at(input);

        this->avatarsInCreation.at(client).race = selectedRace;
        this->creatingClients.at(client) = AvatarCreationStage::Trait1;

        message << Avatar::raceToStringMap.at(selectedRace) << "\n"
                << "Enter a trait of your character:\n";

        return message.str();
    }

    std::string
    AvatarHandler::processFirstTrait(const Connection &client, const std::string &input) {
        std::ostringstream message;

        if (input.empty()) {
            message << "Invalid input, please try again.\n"
                    << "Enter a trait of your character:\n";
            return message.str();
        }

        this->avatarsInCreation.at(client).trait1 = input;
        this->creatingClients.at(client) = AvatarCreationStage::Trait2;

        message << input << "\n"
                << "Enter a second trait of your character:\n";
        return message.str();
    }

    std::string
    AvatarHandler::processSecondTrait(const Connection &client, const std::string &input) {
        std::ostringstream message;

        if (input.empty()) {
            message << "Invalid input, please try again.\n"
                    << "Enter a second trait of your character:\n";
            return message.str();
        }

        this->avatarsInCreation.at(client).trait2 = input;
        this->creatingClients.at(client) = AvatarCreationStage::Confirm;

        auto avatar = this->avatarsInCreation.at(client);
        message << input << "\n"
                << "\n"
                << "Gender: "  << Avatar::genderToStringMap.at(avatar.gender) << "\n"
                << "Race: "    << Avatar::raceToStringMap.at(avatar.race) << "\n"
                << "Trait 1: " << avatar.trait1 << "\n"
                << "Trait 2: " << avatar.trait2 << "\n"
                << "Is the above description of your character correct? [Yes/No]\n";
        return message.str();
    }


    std::string
    AvatarHandler::processConfirm(const Connection &client, const std::string &input) {
        std::ostringstream message;
        if (input == "y" || input == "yes") {
            auto avatar = this->avatarsInCreation.at(client);
            auto player = this->accountHandler.getPlayerByClient(client);
            player->setAvatar(avatar);
            this->exitCreation(client);
            DataManager::saveRegisteredUser(*player);

            message << input << "\n"
                    << "Your character has been successfully created!\n";

        } else if (input == "n" || input == "no") {
            this->creatingClients.at(client) = AvatarCreationStage::Gender;
            message << input << "\n"
                    << "Enter the gender of your character: [Male/Female]\n";

        } else {
            auto avatar = this->avatarsInCreation.at(client);
            message << "Invalid input, please try again.\n"
                    << "\n"
                    << "Gender: "  << Avatar::genderToStringMap.at(avatar.gender) << "\n"
                    << "Race: "    << Avatar::raceToStringMap.at(avatar.race) << "\n"
                    << "Trait 1: " << avatar.trait1 << "\n"
                    << "Trait 2: " << avatar.trait2 << "\n"
                    << "Is the above description of your character correct? [Yes/No]\n";
        }

        return message.str();
    }


    bool
    AvatarHandler::isCreatingAvatar(const Connection &client) const {
        return static_cast<bool>(this->creatingClients.count(client));
    }


    std::string
    AvatarHandler::processCreation(const Connection &client, const std::string &input) {
        std::ostringstream response;
        auto lowercaseInput = boost::algorithm::to_lower_copy(input);

        if (!isCreatingAvatar(client)) {
            // Start avatar creation process
            this->creatingClients.emplace(client, AvatarCreationStage::Gender);
            this->avatarsInCreation.emplace(client, Avatar{});
            response << "\n"
                     << "Avatar Creation\n"
                     << "---------------\n"
                     << "Enter the gender of your character: [Male/Female]\n";

            return response.str();
        }

        auto creationStage = this->creatingClients.at(client);

        switch (creationStage) {
            case AvatarCreationStage::Gender: {
                response << this->processGender(client, lowercaseInput);
                break;
            }

            case AvatarCreationStage::Race: {
                response << this->processRace(client, lowercaseInput);
                break;
            }

            case AvatarCreationStage::Trait1: {
                response << this->processFirstTrait(client, lowercaseInput);
                break;
            }

            case AvatarCreationStage::Trait2: {
                response << this->processSecondTrait(client, lowercaseInput);
                break;
            }

            case AvatarCreationStage::Confirm: {
                response << this->processConfirm(client, lowercaseInput);
                break;
            }

            default: {
                throw std::runtime_error("Error: stage in avatar creation does not exist");
            }
        }

        return response.str();
    }

    void
    AvatarHandler::exitCreation(const Connection &client) {
        this->creatingClients.erase(client);
        this->avatarsInCreation.erase(client);
    }
}
