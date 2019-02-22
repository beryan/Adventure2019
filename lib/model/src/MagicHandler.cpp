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
    MagicHandler::MagicHandler(PlayerHandler* playerHandler) {
        this->playerHandler = playerHandler;
        this->swapTracker = {};
        this->confuseTracker = {};
    }


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
            responseMessage << "There are no spells with the name of \"" << spellName << "\"\n";

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

        auto casterUsername = this->playerHandler->getUsernameByClient(client);
        auto casterPlayerId = this->playerHandler->getPlayerIdByClient(client);

        if (targetName.empty()) {
            return {{client, "You need to specify the name of the person to cast swap on.\n"}};
        }

        if (casterUsername == targetName) {
            return {{client, "You can't cast Swap on yourself!\n"}};
        }

        std::ostringstream casterMessage;
        std::ostringstream targetMessage;

        try {
            auto targetClient = this->playerHandler->getClientByUsername(targetName);
            auto targetPlayerId = this->playerHandler->getPlayerIdByClient(targetClient);
            auto targetRoomId = this->playerHandler->getRoomIdByClient(targetClient);
            auto casterRoomId = this->playerHandler->getRoomIdByClient(client);

            if (targetRoomId != casterRoomId) {
                casterMessage << "There is no one here with the name \"" << targetName << "\"\n";
                return {{client, casterMessage.str()}};
            }

            this->playerHandler->swapPlayerClientsByPlayerId(casterPlayerId, targetPlayerId);
            this->swapTracker.push_back({casterPlayerId, targetPlayerId, SWAP_DURATION});

            casterMessage << "You have successfully swapped bodies with " << targetName << "\n";
            targetMessage << casterUsername << " cast swap on you!\n";

            responses.push_back({targetClient, targetMessage.str()});
            responses.push_back({client, casterMessage.str()});

        } catch (std::out_of_range &error) {
            casterMessage << "There is no one here with the name \"" << targetName << "\"\n";
            responses.push_back({client, casterMessage.str()});
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
        if (targetName.empty()) {
            return {{client, "You need to specify the name of the person to cast swap on.\n"}};
        }

        std::vector<Message> responses;

        try {
            auto casterPlayerId = this->playerHandler->getPlayerIdByClient(client);
            auto targetClient = this->playerHandler->getClientByUsername(targetName);
            auto targetPlayerId = this->playerHandler->getPlayerIdByClient(targetClient);

            auto casterRoomId = this->playerHandler->getRoomIdByClient(client);
            auto targetRoomId = this->playerHandler->getRoomIdByClient(targetClient);

            if (casterRoomId != targetRoomId) {
                return {{client, "There is no player here with the name \"" + targetName + "\"\n"}};
            }

            this->confuseTracker.push_back({casterPlayerId, targetPlayerId, CONFUSE_DURATION});

            auto casterUsername = this->playerHandler->getUsernameByClient(client);

            if (casterUsername == targetName) {
                return {{client, "You cast Confuse on yourself.\n"}};
            }

            std::ostringstream casterMessage;
            std::ostringstream targetMessage;

            casterMessage << "You cast Confuse on " << targetName << "\n";
            targetMessage << casterUsername << " cast Confuse on you!" << "\n";

            responses.push_back({client, casterMessage.str()});
            responses.push_back({targetClient, targetMessage.str()});

        } catch (std::out_of_range &error) {
            responses.push_back({client, "There is no player here with the name \"" + targetName + "\"\n"});
        }

        return responses;
    }

    bool
    MagicHandler::isConfused(const Connection &client) {
        auto playerId = this->playerHandler->getPlayerIdByClient(client);

        auto it_confuse = std::find_if(
                this->confuseTracker.begin(),
                this->confuseTracker.end(),
                [&playerId](const SpellInstance &confuseInstance) {
                    return playerId == confuseInstance.targetPlayerId;
                }
        );

        return it_confuse != this->confuseTracker.end();
    }


    std::string
    MagicHandler::confuseSpeech(std::string message) {
        boost::find_format_all(
            message,
            boost::token_finder((boost::is_alpha() || boost::is_any_of("'")), boost::token_compress_on),
            PigLatinFormatter<std::string>()
        );

        return message;
    }


    void
    MagicHandler::handleLogout(const Connection &client) {
        // Swap back players if client is under Swap spell effects
        auto clientPlayerId = this->playerHandler->getPlayerIdByClient(client);
        auto it_swap = std::find_if(
            this->swapTracker.begin(),
            this->swapTracker.end(),
            [&clientPlayerId](const SpellInstance &swapInstance) {
                return (clientPlayerId == swapInstance.casterPlayerId || clientPlayerId == swapInstance.targetPlayerId);
            }
        );

        if (it_swap != this->swapTracker.end()) {
            auto casterPlayerId= it_swap->casterPlayerId;
            auto targetPlayerId = it_swap->targetPlayerId;

            this->playerHandler->swapPlayerClientsByPlayerId(casterPlayerId, targetPlayerId);
            this->swapTracker.erase(it_swap);
        }

        // Remove Confuse spell effect from player
        auto playerId = this->playerHandler->getPlayerIdByClient(client);
        auto it_confuse = std::find_if(
            this->confuseTracker.begin(),
            this->confuseTracker.end(),
            [&playerId](const SpellInstance &confuseInstance) {
                return playerId == confuseInstance.targetPlayerId;
            }
        );

        if (it_confuse != this->confuseTracker.end()) {
            this->confuseTracker.erase(it_confuse);
        }
    }


    void
    MagicHandler::processCycle(std::deque<Message> &messages) {

       // Handle swap spell expiration
       auto swapInstance = this->swapTracker.begin();

       while (swapInstance != this->swapTracker.end()) {
           if (swapInstance->cyclesRemaining > 0) {
               --swapInstance->cyclesRemaining;
               ++swapInstance;

           } else {
               auto casterPlayerId = swapInstance->casterPlayerId;
               auto targetPlayerId = swapInstance->targetPlayerId;

               this->playerHandler->swapPlayerClientsByPlayerId(casterPlayerId, targetPlayerId);

               auto casterClient = this->playerHandler->getClientByPlayerId(casterPlayerId);
               auto targetClient = this->playerHandler->getClientByPlayerId(targetPlayerId);

               std::string message = "The effects of Swap has worn off and you return to your original body.\n";
               messages.push_back({casterClient, message});
               messages.push_back({targetClient, message});

               this->swapTracker.erase(swapInstance);
           }
       }

       // Handle confuse spell expiration
       auto confuseInstance = this->confuseTracker.begin();

       while (confuseInstance != this->confuseTracker.end()) {
           if (confuseInstance->cyclesRemaining > 0) {
               --confuseInstance->cyclesRemaining;
               ++confuseInstance;

           } else {
               auto targetClient = this->playerHandler->getClientByPlayerId(confuseInstance->targetPlayerId);

               std::string message = "The effects of Confuse has worn off and your speech returns to normal.\n";
               messages.push_back({targetClient, message});

               this->confuseTracker.erase(confuseInstance);
           }
       }
    }
}