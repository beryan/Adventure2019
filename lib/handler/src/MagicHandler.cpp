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

using handler::MagicHandler;

namespace handler {
    MagicHandler::MagicHandler(AccountHandler &accountHandler, CombatHandler &combatHandler) :
        accountHandler(accountHandler),
        combatHandler(combatHandler){}


    std::string MagicHandler::getSpells() {
        std::ostringstream spells;

        spells << "\n"
               << "Spells:\n"
               << "-------\n"
               << "  - " << CONFUSE_SPELL_NAME
               << " (causes the target player to temporarily speak in Pig Latin)\n"

               << "  - " << BODY_SWAP_SPELL_NAME
               << " (causes the caster to switch bodies with the target player temporarily)\n"

               << "  - " << DECOY_SPELL_NAME
               << " (creates a decoy of yourself in combat to escape)\n"

               << "  - " << HEAL_SPELL_NAME
               << " (fully restores target player's health. Can not be used if caster or target is in combat)\n";

        return spells.str();
    }


    std::vector<Message>
    MagicHandler::castSpell(const Connection &client, const std::string &spellName, const std::string& targetName) {
        std::string spellNameLower = boost::algorithm::to_lower_copy(spellName);
        if (spellName.empty()) {
            return {{client, "You need to specify the name of the spell to be cast.\n"}};
        }

        std::vector<Message> responses;
        bool isValidSpellName = static_cast<bool>(this->spellMap.count(spellNameLower));

        if (!isValidSpellName) {
            std::ostringstream responseMessage;
            responseMessage << "There are no spells with the name of \"" << spellName << "\".\n";

            return {{client, responseMessage.str()}};
        }

        auto spell = this->spellMap.at(spellNameLower);
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

            case (Spell::Heal):
                responses = this->heal(client, targetName);
                break;

            default:
                break;
        }

        return responses;
    }


    std::vector<Message>
    MagicHandler::bodySwap(const Connection &client, const std::string &targetName) {
        std::vector<Message> responses;
        std::ostringstream casterMessage;

        if (this->isBodySwapped(client)) {
            casterMessage << "You can't cast " << BODY_SWAP_SPELL_NAME << " while already under the effects of the spell!\n";
            return {{client, casterMessage.str()}};
        }

        if (targetName.empty()) {
            casterMessage << "You need to specify the name of the person to cast " << BODY_SWAP_SPELL_NAME << " on.\n";
            return {{client, casterMessage.str()}};
        }

        auto casterPlayerId = this->accountHandler.getPlayerIdByClient(client);
        auto casterRoomId = this->accountHandler.getRoomIdByClient(client);
        auto casterUsername = this->accountHandler.getUsernameByClient(client);
        if (casterUsername == targetName) {
            casterMessage << "You can't cast " << BODY_SWAP_SPELL_NAME << " on yourself!\n";
            return {{client, casterMessage.str()}};
        }

        auto targetClient = this->accountHandler.getClientByUsername(targetName);
        if (targetClient.id == AccountHandler::INVALID_ID) {
            casterMessage << "There is no one here with the name \"" << targetName << "\".\n";
            return {{client, casterMessage.str()}};
        }

        auto targetRoomId = this->accountHandler.getRoomIdByClient(targetClient);
        if (targetRoomId != casterRoomId) {
            casterMessage << "There is no one here with the name \"" << targetName << "\".\n";
            return {{client, casterMessage.str()}};
        }

        if (this->isBodySwapped(targetClient)) {
            casterMessage << targetName << " is already under the effects of the " << BODY_SWAP_SPELL_NAME << " spell!\n";
            return {{client, casterMessage.str()}};
        }

        auto targetPlayerId = this->accountHandler.getPlayerIdByClient(targetClient);
        this->accountHandler.swapPlayerClientsByPlayerId(casterPlayerId, targetPlayerId);
        this->bodySwapInstances.push_back({casterPlayerId, targetPlayerId, BODY_SWAP_DURATION});

        casterMessage << "You have successfully swapped bodies with " << targetName << ".\n";

        std::ostringstream targetMessage;
        targetMessage << casterUsername << " casts " << BODY_SWAP_SPELL_NAME << " on you!\n";

        responses.push_back({targetClient, targetMessage.str()});
        responses.push_back({client, casterMessage.str()});

        return responses;
    }


    std::vector<Message>
    MagicHandler::decoy(const Connection &client) {
        std::vector<Message> responses;
        std::ostringstream message;
        auto player = this->accountHandler.getPlayerByClient(client);

        if (!this->combatHandler.isInCombat(*player)) {
            message << "You can only cast " << DECOY_SPELL_NAME << " while in combat.\n";
            return {{client, message.str()}};
        }

        this->combatHandler.replaceWithDecoy(*player);

        message << "You create a decoy of yourself and flee from combat.\n";
        responses.push_back({client, message.str()});

        return responses;
    }


    std::vector<Message>
    MagicHandler::confuse(const Connection &client, const std::string &targetName) {
        std::vector<Message> responses;
        std::ostringstream casterMessage;

        if (targetName.empty()) {
            casterMessage << "You need to specify the name of the person to cast " << CONFUSE_SPELL_NAME << " on.\n";
            return {{client, casterMessage.str()}};
        }

        auto casterPlayerId = this->accountHandler.getPlayerIdByClient(client);
        auto casterRoomId = this->accountHandler.getRoomIdByClient(client);
        auto casterUsername = this->accountHandler.getUsernameByClient(client);
        if (casterUsername == targetName) {
            if (this->isConfused(client)) {
                casterMessage << "You are already under the effects of the " << CONFUSE_SPELL_NAME << " spell!\n";
                return {{client, casterMessage.str()}};
            }

            this->confuseInstances.push_back({casterPlayerId, casterPlayerId, CONFUSE_DURATION});

            casterMessage << "You cast " << CONFUSE_SPELL_NAME << " on yourself.\n";
            return {{client, casterMessage.str()}};
        }

        auto targetClient = this->accountHandler.getClientByUsername(targetName);
        if (targetClient.id == AccountHandler::INVALID_ID) {
            casterMessage << "There is no player here with the name \"" << targetName << "\".\n";
            return {{client, casterMessage.str()}};
        }

        auto targetPlayerId = this->accountHandler.getPlayerIdByClient(targetClient);
        auto targetRoomId = this->accountHandler.getRoomIdByClient(targetClient);
        if (casterRoomId != targetRoomId) {
            casterMessage << "There is no player here with the name \"" << targetName << "\".\n";
            return {{client, casterMessage.str()}};
        }

        if (isConfused(targetClient)) {
            casterMessage << targetName << " is already under the effects of the " << CONFUSE_SPELL_NAME << " spell!\n";
            return {{client, casterMessage.str()}};
        }

        this->confuseInstances.push_back({casterPlayerId, targetPlayerId, CONFUSE_DURATION});

        casterMessage << "You cast " << CONFUSE_SPELL_NAME << " on " << targetName << ".\n";

        std::ostringstream targetMessage;
        targetMessage << casterUsername << " casts " << CONFUSE_SPELL_NAME << " on you!\n";

        responses.push_back({client, casterMessage.str()});
        responses.push_back({targetClient, targetMessage.str()});

        return responses;
    }


    std::vector<Message>
    MagicHandler::heal(const Connection &client, const std::string &targetName) {
        std::vector<Message> responses;
        std::ostringstream casterMessage;

        auto player = accountHandler.getPlayerByClient(client);
        auto casterUsername = this->accountHandler.getUsernameByClient(client);

        if (combatHandler.isInCombat(*player)) {
            casterMessage << "You can't cast " << HEAL_SPELL_NAME << " while in combat.\n";
            return {{client, casterMessage.str()}};
        }

        if (targetName.empty() || (casterUsername == targetName)) {
            player->setHealth(Character::STARTING_HEALTH);

            casterMessage << "You cast " << HEAL_SPELL_NAME << " on yourself.\n";
            return {{client, casterMessage.str()}};
        }

        auto targetClient = this->accountHandler.getClientByUsername(targetName);
        if (targetClient.id == AccountHandler::INVALID_ID) {
            casterMessage << "There is no player here with the name \"" << targetName << "\".\n";
            return {{client, casterMessage.str()}};
        }

        auto targetRoomId = this->accountHandler.getRoomIdByClient(targetClient);
        auto casterRoomId = this->accountHandler.getRoomIdByClient(client);
        if (casterRoomId != targetRoomId) {
            casterMessage << "There is no player here with the name \"" << targetName << "\".\n";
            return {{client, casterMessage.str()}};
        }

        auto targetPlayer = this->accountHandler.getPlayerByClient(targetClient);
        if (combatHandler.isInCombat(*targetPlayer)) {
            casterMessage << "You can't cast " << HEAL_SPELL_NAME << " on "
                          << targetPlayer->getUsername() << " while they are in combat.\n";
            return {{client, casterMessage.str()}};
        }

        targetPlayer->setHealth(Character::STARTING_HEALTH);

        casterMessage << "You cast " << HEAL_SPELL_NAME << " on " << targetName << ".\n";

        std::ostringstream targetMessage;
        targetMessage << casterUsername << " casts " << HEAL_SPELL_NAME << " on you!\n";

        responses.push_back({client, casterMessage.str()});
        responses.push_back({targetClient, targetMessage.str()});

        return responses;
    }


    void
    MagicHandler::removeBodySwap(const model::ID &playerId) {
        auto it_swap = std::find_if(
            this->bodySwapInstances.begin(),
            this->bodySwapInstances.end(),
            [&playerId](const SpellInstance &swapInstance) {
                return (playerId == swapInstance.casterPlayerId) || (playerId == swapInstance.targetPlayerId);
            }
        );

        if (it_swap != this->bodySwapInstances.end()) {
            auto casterPlayerId= it_swap->casterPlayerId;
            auto targetPlayerId = it_swap->targetPlayerId;

            this->accountHandler.swapPlayerClientsByPlayerId(casterPlayerId, targetPlayerId);
            this->bodySwapInstances.erase(it_swap);
        }
    }


    void
    MagicHandler::removeConfuse(const model::ID &playerId) {
        auto it_confuse = std::find_if(
            this->confuseInstances.begin(),
            this->confuseInstances.end(),
            [&playerId](const SpellInstance &confuseInstance) {
                return (playerId == confuseInstance.casterPlayerId) || (playerId == confuseInstance.targetPlayerId);
            }
        );

        if (it_confuse != this->confuseInstances.end()) {
            this->confuseInstances.erase(it_confuse);
        }
    }


    void
    MagicHandler::processBodySwapInstancesCycle(std::deque<Message> &messages) {
        auto swapInstance = this->bodySwapInstances.begin();

        while (swapInstance != this->bodySwapInstances.end()) {
            if (swapInstance->cyclesRemaining > 0) {
                --swapInstance->cyclesRemaining;
                ++swapInstance;

            } else {
                auto casterPlayerId = swapInstance->casterPlayerId;
                auto targetPlayerId = swapInstance->targetPlayerId;

                this->accountHandler.swapPlayerClientsByPlayerId(casterPlayerId, targetPlayerId);

                auto casterClient = this->accountHandler.getClientByPlayerId(casterPlayerId);
                auto targetClient = this->accountHandler.getClientByPlayerId(targetPlayerId);

                std::ostringstream message;
                message << "The effects of " << BODY_SWAP_SPELL_NAME << " have worn off and you return to your original body.\n";
                messages.push_back({casterClient, message.str()});
                messages.push_back({targetClient, message.str()});

                this->bodySwapInstances.erase(swapInstance);
            }
        }
    }


    void
    MagicHandler::processConfuseInstancesCycle(std::deque<Message> &messages) {
        auto confuseInstance = this->confuseInstances.begin();

        while (confuseInstance != this->confuseInstances.end()) {
            if (confuseInstance->cyclesRemaining > 0) {
                --confuseInstance->cyclesRemaining;
                ++confuseInstance;

            } else {
                auto targetClient = this->accountHandler.getClientByPlayerId(confuseInstance->targetPlayerId);

                std::ostringstream message;
                message << "The effects of " << CONFUSE_SPELL_NAME << " have worn off and your speech returns to normal.\n";
                messages.push_back({targetClient, message.str()});

                this->confuseInstances.erase(confuseInstance);
            }
        }
    }


    bool
    MagicHandler::isBodySwapped(const Connection &client) {
        auto playerId = this->accountHandler.getPlayerIdByClient(client);

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
        auto playerId = this->accountHandler.getPlayerIdByClient(client);

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
        auto playerId = this->accountHandler.getPlayerIdByClient(client);

        if (this->isBodySwapped(client)) {
            this->removeBodySwap(playerId);
        }

        if (this->isConfused(client)) {
            this->removeConfuse(playerId);
        }
    }


    void
    MagicHandler::processCycle(std::deque<Message> &messages) {
       this->processBodySwapInstancesCycle(messages);
       this->processConfuseInstancesCycle(messages);
    }
}