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

                this->avatarsInCreation.at(client).race = lowercaseInput;
                this->creatingClients.at(client) = AvatarCreationStage::Trait1;

                response << lowercaseInput << "\n"
                         << "Enter a trait of your character:\n";
                break;
            }

            case AvatarCreationStage::Trait1: {
                if (lowercaseInput.empty()) {
                    response << "Invalid input, please try again.\n"
                             << "Enter a trait of your character:\n";
                    break;
                }

                this->avatarsInCreation.at(client).trait1 = lowercaseInput;
                this->creatingClients.at(client) = AvatarCreationStage::Trait2;

                response << lowercaseInput << "\n"
                         << "Enter a second trait of your character:\n";
                break;
            }

            case AvatarCreationStage::Trait2: {
                if (lowercaseInput.empty()) {
                    response << "Invalid input, please try again.\n"
                             << "Enter a second trait of your character:\n";
                    break;
                }

                this->avatarsInCreation.at(client).trait2 = lowercaseInput;
                this->creatingClients.at(client) = AvatarCreationStage::Confirm;

                auto avatar = this->avatarsInCreation.at(client);
                response << lowercaseInput << "\n"
                         << "\n"
                         << "Gender: "  << avatar.gender << "\n"
                         << "Race: "    << avatar.race << "\n"
                         << "Trait 1: " << avatar.trait1 << "\n"
                         << "Trait 2: " << avatar.trait2 << "\n"
                         << "Is the above description of your character correct? [Yes/No]\n";

                break;
            }


            case AvatarCreationStage::Confirm: {
                if (lowercaseInput == "y" || lowercaseInput == "yes") {
                    auto avatar = this->avatarsInCreation.at(client);
                    auto player = this->accountHandler->getPlayerByClient(client);
                    player->setAvatar(avatar);
                    this->exitCreation(client);

                    response << input << "\n"
                             << "Your character has been successfully created!\n";

                } else if (lowercaseInput == "n" || lowercaseInput == "no") {
                    this->creatingClients.at(client) = AvatarCreationStage::Gender;
                    response << input << "\n"
                             << "Enter the gender of your character: [Male/Female]\n";

                } else {
                    auto avatar = this->avatarsInCreation.at(client);
                    response << "Invalid input, please try again.\n"
                             << "\n"
                             << "Gender: "  << avatar.gender << "\n"
                             << "Race: "    << avatar.race << "\n"
                             << "Trait 1: " << avatar.trait1 << "\n"
                             << "Trait 2: " << avatar.trait2 << "\n"
                             << "Is the above description of your character correct? [Yes/No]\n";
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
