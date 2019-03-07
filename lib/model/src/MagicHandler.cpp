//
// Created by louis on 16/02/19.
//

#include "MagicHandler.h"
#include <sstream>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/find_format.hpp>
#include <boost/algorithm/string/formatter.hpp>

using model::MagicHandler;

namespace model {
    MagicHandler::MagicHandler(AccountHandler &accountHandler) :
        accountHandler(&accountHandler){}


    std::vector<Message>
    MagicHandler::castSpell(const Connection &client, const std::string &param) {
        std::vector<std::string> arguments;
        boost::algorithm::split(arguments, param, boost::is_any_of(" "), boost::token_compress_on);

        auto spellName = boost::algorithm::to_lower_copy(arguments.at(0));
        if (spellName.empty()) {
            return {{client, "You need to specify the name of the spell to be cast.\n"}};
        }

        std::string targetName;
        if (arguments.size() > 1) {
            targetName = arguments.at(1);
        }

        std::vector<Message> responses;
        bool isValidSpellName = static_cast<bool>(this->spellMap.count(spellName));

        if (!isValidSpellName) {
            std::ostringstream responseMessage;
            responseMessage << "There are no spells with the name of \"" << arguments.at(0) << "\"\n";

            return {{client, responseMessage.str()}};
        }

        auto spell = this->spellMap.at(spellName);
        switch (spell) {
            case (Spell::BodySwap):
                responses = this->bodySwap(client, targetName);
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
    MagicHandler::bodySwap(const Connection &client, const std::string &targetName) {
        std::vector<Message> responses;

        if (this->isBodySwapped(client)) {
            return {{client, "You can't cast Swap while already under the effects of the spell!\n"}};
        }

        if (targetName.empty()) {
            return {{client, "You need to specify the name of the person to cast Swap on.\n"}};
        }

        auto casterPlayerId = this->accountHandler->getPlayerIdByClient(client);
        auto casterRoomId = this->accountHandler->getRoomIdByClient(client);
        auto casterUsername = this->accountHandler->getUsernameByClient(client);
        if (casterUsername == targetName) {
            return {{client, "You can't cast Swap on yourself!\n"}};
        }

        std::ostringstream casterMessage;
        std::ostringstream targetMessage;

        auto targetClient = this->accountHandler->getClientByUsername(targetName);
        if (targetClient.id == 0) {
            casterMessage << "There is no one here with the name \"" << targetName << "\"\n";
            return {{client, casterMessage.str()}};
        }

        auto targetRoomId = this->accountHandler->getRoomIdByClient(targetClient);
        if (targetRoomId != casterRoomId) {
            casterMessage << "There is no one here with the name \"" << targetName << "\"\n";
            return {{client, casterMessage.str()}};
        }

        if (this->isBodySwapped(targetClient)) {
            casterMessage << targetName << " is already under the effects of the Swap spell!\n";
            return {{client, casterMessage.str()}};
        }

        auto targetPlayerId = this->accountHandler->getPlayerIdByClient(targetClient);
        this->accountHandler->swapPlayerClientsByPlayerId(casterPlayerId, targetPlayerId);
        this->bodySwapInstances.push_back({casterPlayerId, targetPlayerId, BODY_SWAP_DURATION});

        casterMessage << "You have successfully swapped bodies with " << targetName << "\n";
        targetMessage << casterUsername << " cast swap on you!\n";

        responses.push_back({targetClient, targetMessage.str()});
        responses.push_back({client, casterMessage.str()});

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

        if (targetName.empty()) {
            return {{client, "You need to specify the name of the person to cast Confuse on.\n"}};
        }

        auto casterPlayerId = this->accountHandler->getPlayerIdByClient(client);
        auto casterRoomId = this->accountHandler->getRoomIdByClient(client);
        auto casterUsername = this->accountHandler->getUsernameByClient(client);
        if ((casterUsername == targetName) && (this->isConfused(client))) {
            return {{client, "You are already under the effects of the Confuse spell!\n"}};
        }

        if (casterUsername == targetName) {
            this->confuseInstances.push_back({casterPlayerId, casterPlayerId, CONFUSE_DURATION});
            return {{client, "You cast Confuse on yourself.\n"}};
        }

        std::ostringstream casterMessage;
        std::ostringstream targetMessage;

        auto targetClient = this->accountHandler->getClientByUsername(targetName);
        if (targetClient.id == 0) {
            casterMessage << "There is no player here with the name \"" << targetName << "\"\n";
            return {{client, casterMessage.str()}};
        }

        auto targetPlayerId = this->accountHandler->getPlayerIdByClient(targetClient);
        auto targetRoomId = this->accountHandler->getRoomIdByClient(targetClient);
        if (casterRoomId != targetRoomId) {
            casterMessage << "There is no player here with the name \"" << targetName << "\"\n";
            return {{client, casterMessage.str()}};
        }

        if (isConfused(targetClient)) {
            casterMessage << targetName << " is already under the effects of the Confuse spell!\n";
            return {{client, casterMessage.str()}};
        }

        this->confuseInstances.push_back({casterPlayerId, targetPlayerId, CONFUSE_DURATION});

        casterMessage << "You cast Confuse on " << targetName << "\n";
        targetMessage << casterUsername << " cast Confuse on you!" << "\n";

        responses.push_back({client, casterMessage.str()});
        responses.push_back({targetClient, targetMessage.str()});

        return responses;
    }


    bool
    MagicHandler::isBodySwapped(const Connection &client) {
        auto playerId = this->accountHandler->getPlayerIdByClient(client);

        auto it_swap = std::find_if(
            this->bodySwapInstances.begin(),
            this->bodySwapInstances.end(),
            [&playerId](const SpellInstance &swapInstance) {
                return (playerId == swapInstance.casterPlayerId) || (playerId == swapInstance.targetPlayerId);
            }
        );

        return it_swap != this->bodySwapInstances.end();
    }


    bool
    MagicHandler::isConfused(const Connection &client) {
        auto playerId = this->accountHandler->getPlayerIdByClient(client);

        auto it_confuse = std::find_if(
            this->confuseInstances.begin(),
            this->confuseInstances.end(),
            [&playerId](const SpellInstance &confuseInstance) {
                return playerId == confuseInstance.targetPlayerId;
            }
        );

        return it_confuse != this->confuseInstances.end();
    }


    void
    MagicHandler::confuseSpeech(std::string &message) {
        boost::find_format_all(
            message,
            boost::token_finder((boost::is_alpha() || boost::is_any_of("'")), boost::token_compress_on),
            PigLatinFormatter<std::string>()
        );
    }


    void
    MagicHandler::handleLogout(const Connection &client) {
        // Swap back players if client is under Swap spell effects
        auto clientPlayerId = this->accountHandler->getPlayerIdByClient(client);
        auto it_swap = std::find_if(
            this->bodySwapInstances.begin(),
            this->bodySwapInstances.end(),
            [&clientPlayerId](const SpellInstance &swapInstance) {
                return (clientPlayerId == swapInstance.casterPlayerId || clientPlayerId == swapInstance.targetPlayerId);
            }
        );

        if (it_swap != this->bodySwapInstances.end()) {
            auto casterPlayerId= it_swap->casterPlayerId;
            auto targetPlayerId = it_swap->targetPlayerId;

            this->accountHandler->swapPlayerClientsByPlayerId(casterPlayerId, targetPlayerId);
            this->bodySwapInstances.erase(it_swap);
        }

        // Remove Confuse spell effect from player
        auto playerId = this->accountHandler->getPlayerIdByClient(client);
        auto it_confuse = std::find_if(
            this->confuseInstances.begin(),
            this->confuseInstances.end(),
            [&playerId](const SpellInstance &confuseInstance) {
                return playerId == confuseInstance.targetPlayerId;
            }
        );

        if (it_confuse != this->confuseInstances.end()) {
            this->confuseInstances.erase(it_confuse);
        }
    }


    void
    MagicHandler::processCycle(std::deque<Message> &messages) {
       // Handle swap spell expiration
       auto swapInstance = this->bodySwapInstances.begin();

       while (swapInstance != this->bodySwapInstances.end()) {
           if (swapInstance->cyclesRemaining > 0) {
               --swapInstance->cyclesRemaining;
               ++swapInstance;

           } else {
               auto casterPlayerId = swapInstance->casterPlayerId;
               auto targetPlayerId = swapInstance->targetPlayerId;

               this->accountHandler->swapPlayerClientsByPlayerId(casterPlayerId, targetPlayerId);

               auto casterClient = this->accountHandler->getClientByPlayerId(casterPlayerId);
               auto targetClient = this->accountHandler->getClientByPlayerId(targetPlayerId);

               std::string message = "The effects of Swap has worn off and you return to your original body.\n";
               messages.push_back({casterClient, message});
               messages.push_back({targetClient, message});

               this->bodySwapInstances.erase(swapInstance);
           }
       }

       // Handle confuse spell expiration
       auto confuseInstance = this->confuseInstances.begin();

       while (confuseInstance != this->confuseInstances.end()) {
           if (confuseInstance->cyclesRemaining > 0) {
               --confuseInstance->cyclesRemaining;
               ++confuseInstance;

           } else {
               auto targetClient = this->accountHandler->getClientByPlayerId(confuseInstance->targetPlayerId);

               std::string message = "The effects of Confuse has worn off and your speech returns to normal.\n";
               messages.push_back({targetClient, message});

               this->confuseInstances.erase(confuseInstance);
           }
       }
    }
}