//
// Created by louis on 16/02/19.
//

#include <sstream>
#include "MagicHandler.h"

using model::MagicHandler;

namespace model {
    MagicHandler::MagicHandler(PlayerHandler* playerHandler) {
        this->playerHandler = playerHandler;
        this->swapTracker = {};
    }


    std::vector<Message>
    MagicHandler::castSpell(const Connection &client, const std::string &spellName, const std::string &targetName) {
        std::vector<Message> responses;
        bool isValidSpellName = static_cast<bool>(this->spellMap.count(spellName));

        if (!isValidSpellName) {
            std::ostringstream responseMessage;
            responseMessage << "There are no spells with the name of " << spellName << "\n";

            return {{client, responseMessage.str()}};
        }

        auto spell = this->spellMap.at(spellName);

        switch (spell) {
            case (Spell::Swap):
                responses = this->swap(client, targetName);
                break;

            case (Spell::Decoy):
                responses = this->decoy(client);
                break;

            case (Spell::Confuse):
                responses = this->confuse(client, targetName);
                break;

            default:
                break;
        }

        return responses;
    }


    std::vector<Message>
    MagicHandler::swap(const Connection &client, const std::string &targetName) {
        std::vector<Message> responses;

        auto casterUsername = this->playerHandler->getUsernameByClient(client);

        if (targetName.empty()) {
            return {{client, "You need to specify the name of the person to cast swap on.\n"}};
        }

        auto swapSuccessful = this->playerHandler->swapPlayerClients(casterUsername, targetName);

        if (swapSuccessful) {
            this->swapTracker.push_back({{casterUsername, targetName}, 30});

            std::ostringstream casterMessage;
            std::ostringstream targetMessage;

            casterMessage << "You have successfully swapped bodies with " << targetName << "\n";
            targetMessage << casterUsername << " cast swap on you!\n";

            auto targetClient = this->playerHandler->getClientByUsername(casterUsername);

            responses.push_back({targetClient, targetMessage.str()});
            responses.push_back({client, casterMessage.str()});

        } else {
            responses.push_back({client, "There is no one here with the name " + targetName + "\n"});
        }

        return responses;
    }


    std::vector<Message>
    MagicHandler::decoy(const Connection &client) {
        std::vector<Message> responses;

        responses.push_back({client, "You do not yet know how to cast the Decoy spell.\n"});

        return responses;
    }


    std::vector<Message>
    MagicHandler::confuse(const Connection &client, const std::string &targetName) {
        std::vector<Message> responses;

        responses.push_back({client, "You do not yet know how to cast the Confuse spell."});

        return responses;
    }


    void
    MagicHandler::handleLogout(const Connection &client) {
        // Swap back players if client is under swap spell effects
        auto clientUsername = this->playerHandler->getUsernameByClient(client);
        auto swapInstance = this->swapTracker.begin();

        while (swapInstance != this->swapTracker.end()) {
            auto casterUsername = swapInstance->first.first;
            auto targetUsername = swapInstance->first.second;

            if (clientUsername == casterUsername || clientUsername == targetUsername) {
                this->playerHandler->swapPlayerClients(casterUsername, targetUsername);
                this->swapTracker.erase(swapInstance);

            } else {
                ++swapInstance;
            }
        }
    }


    void
    MagicHandler::processCycle(std::deque<Message> &messages) {

       // Handle swap spell expiration
       auto swapInstance = this->swapTracker.begin();

       while (swapInstance != this->swapTracker.end()) {
           if (swapInstance->second > 0) {
               --swapInstance->second;
               ++swapInstance;

           } else {
               auto casterUsername = swapInstance->first.first;
               auto targetUsername = swapInstance->first.second;
               this->playerHandler->swapPlayerClients(casterUsername, targetUsername);


               auto casterClient = this->playerHandler->getClientByUsername(casterUsername);
               auto targetClient = this->playerHandler->getClientByUsername(targetUsername);

               std::string message = "The effects of swap has worn off and you return to your original body.\n";
               messages.push_back({casterClient, message});
               messages.push_back({targetClient, message});

               this->swapTracker.erase(swapInstance);
           }
       }
    }
}