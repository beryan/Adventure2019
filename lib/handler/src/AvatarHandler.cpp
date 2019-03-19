//
// Created by louis on 18/03/19.
//

#include <sstream>
#include <boost/algorithm/string/case_conv.hpp>
#include "Player.h"
#include "AvatarHandler.h"

using handler::AvatarHandler;

namespace handler {
    AvatarHandler::AvatarHandler(AccountHandler &accountHandler) {
        this->accountHandler = &accountHandler;
    }

    bool
    AvatarHandler::isCreatingAvatar(const Connection &client) {
        return static_cast<bool>(this->creatingClients.count(client));
    }

    std::string
    AvatarHandler::processCreation(const Connection &client, const std::string &input) {
        std::ostringstream response;
        auto lowercaseInput = std::string(boost::algorithm::to_lower_copy(input));

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
                std::string selectedGender;

                if (lowercaseInput == "m" || lowercaseInput == "male") {
                    selectedGender = "male";

                } else if (lowercaseInput == "f" || lowercaseInput == "female") {
                    selectedGender = "female";

                } else {
                    response << "Invalid input, please try again.\n"
                             << "Enter the gender of your character: [Male/Female]\n";
                    break;
                }

                this->avatarsInCreation.at(client).gender = selectedGender;
                this->creatingClients.at(client) = AvatarCreationStage::Race;

                response << selectedGender << "\n"
                        << "Enter the race of your character: [Human/Dwarf/Elf/Orc]\n";
                break;
            }

            case AvatarCreationStage::Race: {
                if (!(lowercaseInput == "human" || lowercaseInput == "elf" ||
                        lowercaseInput == "dwarf" || lowercaseInput == "orc")) {
                    response << "Invalid input, please try again.\n"
                             << "Enter the race of your character: [Human/Dwarf/Elf/Orc]\n";
                    break;
                }

                this->avatarsInCreation.at(client).race = input;
                this->creatingClients.at(client) = AvatarCreationStage::Trait1;

                response << input << "\n"
                         << "Enter a trait of your character:\n";
                break;
            }

            case AvatarCreationStage::Trait1: {
                if (lowercaseInput.empty()) {
                    response << "Invalid input, please try again.\n"
                             << "Enter a trait of your character:\n";
                    break;
                }

                this->avatarsInCreation.at(client).trait1 = input;
                this->creatingClients.at(client) = AvatarCreationStage::Trait2;

                response << input << "\n"
                         << "Enter a second trait of your character:\n";
                break;
            }

            case AvatarCreationStage::Trait2: {
                if (lowercaseInput.empty()) {
                    response << "Invalid input, please try again.\n"
                             << "Enter a second trait of your character:\n";
                    break;
                }

                this->avatarsInCreation.at(client).trait2 = input;
                this->creatingClients.at(client) = AvatarCreationStage::Confirm;

                auto avatar = this->avatarsInCreation.at(client);
                response << input << "\n"
                         << "Is the following description of your character correct? [Yes/No]\n"
                         << "Gender: "  << avatar.gender << "\n"
                         << "Race: "    << avatar.race << "\n"
                         << "Trait 1: " << avatar.trait1 << "\n"
                         << "Trait 2: " << avatar.trait2 << "\n";

                break;
            }


            case AvatarCreationStage::Confirm: {
                if (lowercaseInput == "y" || lowercaseInput == "yes") {
                    this->creatingClients.erase(client);

                    auto avatar = this->avatarsInCreation.at(client);
                    auto player = this->accountHandler->getPlayerByClient(client);
                    player->setAvatar(avatar);

                    response << "Your character has been successfully created!\n";

                } else {
                    this->creatingClients.at(client) = AvatarCreationStage::Race;
                    response << input << "\n"
                             << "Enter the race of your character:\n";
                }

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
