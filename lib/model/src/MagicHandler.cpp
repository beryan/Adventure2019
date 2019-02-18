//
// Created by louis on 16/02/19.
//

#include <sstream>
#include "MagicHandler.h"
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/find_format.hpp>
#include <boost/algorithm/string/formatter.hpp>

using model::MagicHandler;

namespace model {

    // Formatter for boost::find_format
    template<typename SeqT>
    struct formatPigLatin {
    private:
        typedef SeqT result_type;
        typedef typename SeqT::value_type value_type;

    public:
        template<typename ReplaceT>
        result_type operator()(const ReplaceT& Replace) const {
            std::ostringstream result;
            std::string temp(Replace.begin(), Replace.end());

            auto firstVowelIndex = temp.find_first_of("aeiouAEIOU");

            if (firstVowelIndex != std::string::npos && firstVowelIndex != 0) {
                // Word starts with consonant(s)
                result << temp.substr(firstVowelIndex) << temp.substr(0, firstVowelIndex) << "ay";

            } else if (firstVowelIndex != std::string::npos) {
                // Word starts with a vowel
                result << temp.substr(firstVowelIndex) << temp.substr(0, firstVowelIndex) << "way";

            } else {
                // Word's letters are all consonants
                result << temp << "ay";
            }

            return result.str();
        }
    };


    MagicHandler::MagicHandler(PlayerHandler* playerHandler) {
        this->playerHandler = playerHandler;
        this->swapTracker = {};
        this->confuseTracker = {};
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

        if (casterUsername == targetName) {
            return {{client, "You can't cast Swap on yourself!\n"}};
        }

        std::ostringstream casterMessage;
        std::ostringstream targetMessage;

        try {
            auto targetClient = this->playerHandler->getClientByUsername(targetName);
            auto targetRoomId = this->playerHandler->getRoomIdByClient(targetClient);
            auto casterRoomId = this->playerHandler->getRoomIdByClient(client);

            if (targetRoomId != casterRoomId) {
                casterMessage << "There is no one here with the name \"" << targetName << "\"\n";
                return {{client, casterMessage.str()}};
            }

            this->swapTracker.push_back({{casterUsername, targetName}, SWAP_DURATION});

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
        std::vector<Message> responses;

        try {
            auto targetClient = this->playerHandler->getClientByUsername(targetName);
            auto targetPlayerId = this->playerHandler->getPlayerIdByClient(targetClient);

            auto casterRoomId = this->playerHandler->getRoomIdByClient(client);
            auto targetRoomId = this->playerHandler->getRoomIdByClient(targetClient);

            if (casterRoomId != targetRoomId) {
                return{{client, "There is no player with the name \"" + targetName + "\"\n"}};
            }

            this->confuseTracker.emplace_back(targetPlayerId, CONFUSE_DURATION);

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
            responses.push_back({client, "There is no player with the name \"" + targetName + "\"\n"});
        }

        return responses;
    }

    bool
    MagicHandler::isConfused(const Connection &client) {
        auto playerId = this->playerHandler->getPlayerIdByClient(client);

        auto it_confuse = std::find_if(
                this->confuseTracker.begin(),
                this->confuseTracker.end(),
                [&playerId](const std::pair<ID, unsigned int> &confuseInstance) {
                    return playerId == confuseInstance.first;
                }
        );

        return it_confuse != this->confuseTracker.end();
    }


    std::string
    MagicHandler::confuseSpeech(const Connection &client, std::string message) {
        boost::find_format_all(
            message,
            boost::token_finder((boost::is_alpha() || boost::is_any_of("'")), boost::token_compress_on),
            formatPigLatin<std::string>()
        );

        return message;
    }


    void
    MagicHandler::handleLogout(const Connection &client) {
        // Swap back players if client is under Swap spell effects
        auto clientUsername = this->playerHandler->getUsernameByClient(client);
        auto it_swap = std::find_if(
            this->swapTracker.begin(),
            this->swapTracker.end(),
            [&clientUsername](const std::pair<std::pair<std::string, std::string>, unsigned int> &swapInstance) {
                return (clientUsername == swapInstance.first.first || clientUsername == swapInstance.first.second);
            }
        );

        if (it_swap != this->swapTracker.end()) {
            auto casterUsername = it_swap->first.first;
            auto targetUsername = it_swap->first.second;

            this->playerHandler->swapPlayerClients(casterUsername, targetUsername);
            this->swapTracker.erase(it_swap);
        }

        // Remove Confuse spell effect from player
        auto playerId = this->playerHandler->getPlayerIdByClient(client);
        auto it_confuse = std::find_if(
            this->confuseTracker.begin(),
            this->confuseTracker.end(),
            [&playerId](const std::pair<ID, unsigned int> &confuseInstance) {
                return playerId == confuseInstance.first;
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
           if (swapInstance->second > 0) {
               --swapInstance->second;
               ++swapInstance;

           } else {
               auto casterUsername = swapInstance->first.first;
               auto targetUsername = swapInstance->first.second;

               this->playerHandler->swapPlayerClients(casterUsername, targetUsername);

               auto casterClient = this->playerHandler->getClientByUsername(casterUsername);
               auto targetClient = this->playerHandler->getClientByUsername(targetUsername);

               std::string message = "The effects of Swap has worn off and you return to your original body.\n";
               messages.push_back({casterClient, message});
               messages.push_back({targetClient, message});

               this->swapTracker.erase(swapInstance);
           }
       }

       // Handle confuse spell expiration
       auto confuseInstance = this->confuseTracker.begin();

       while (confuseInstance != this->confuseTracker.end()) {
           if (confuseInstance->second > 0) {
               --confuseInstance->second;
               ++confuseInstance;

           } else {
               auto targetClient = this->playerHandler->getClientByPlayerId(confuseInstance->first);

               std::string message = "The effects of Confuse has worn off and your speech returns to normal.\n";
               messages.push_back({targetClient, message});

               this->confuseTracker.erase(confuseInstance);
           }
       }
    }
}