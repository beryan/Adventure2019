#ifndef MAGICHANDLER_H
#define MAGICHANDLER_H

#include "AccountHandler.h"
#include "CombatHandler.h"
#include <sstream>

using handler::AccountHandler;
using handler::CombatHandler;
using model::Player;
using model::ID;

namespace handler {
    /**
     *  A spell instance containing the Player ID of the spell's caster and target, along
     *  with the remaining duration of the spell (in number of game cycles).
     */
    struct SpellInstance {
        ID casterPlayerId;
        ID targetPlayerId;
        unsigned int cyclesRemaining;
    };

    /**
     * Formatter for boost::find_format. Converts strings into Pig Latin.
     */
    template<typename SeqT>
    struct PigLatinFormatter {
        template<typename ReplaceT>
        SeqT operator()(const ReplaceT &Replace) const {
            std::ostringstream result;
            std::string word(Replace.begin(), Replace.end());

            auto firstVowelIndex = word.find_first_of("aeiouAEIOU");

            if (firstVowelIndex == std::string::npos) {
                // Word's letters are all consonants
                result << word << "ay";

            } else if (firstVowelIndex == 0) {
                // Word starts with a vowel
                result << word << "way";

            } else {
                // Word starts with consonant(s)
                result << word.substr(firstVowelIndex) << word.substr(0, firstVowelIndex) << "ay";
            }

            return result.str();
        }
    };


    constexpr auto CONFUSE_SPELL_NAME = "confuse";
    constexpr auto BODY_SWAP_SPELL_NAME = "swap";
    constexpr auto DECOY_SPELL_NAME = "decoy";
    constexpr auto HEAL_SPELL_NAME = "heal";
    constexpr unsigned int BODY_SWAP_DURATION = 50;
    constexpr unsigned int CONFUSE_DURATION = 50;

    /**
     *  @class MagicHandler
     *
     *  @brief A class for defining magic spells and performing the operations involved.
     *
     *  This class defines the available magic spells in the game, their effects on
     *  characters, and tracks the longevity of each spell instance. It uses the
     *  Game class' AccountHandler to associate instances of spells on active players.
     */
    class MagicHandler {
    private:
        AccountHandler& accountHandler;
        CombatHandler& combatHandler;
        std::vector<SpellInstance> bodySwapInstances;
        std::vector<SpellInstance> confuseInstances;

        enum class Spell {
            BodySwap,
            Confuse,
            Decoy,
            Heal
        };

        std::map<std::string, Spell> spellMap = {
                {BODY_SWAP_SPELL_NAME, Spell::BodySwap},
                {CONFUSE_SPELL_NAME,   Spell::Confuse},
                {DECOY_SPELL_NAME,     Spell::Decoy},
                {HEAL_SPELL_NAME,      Spell::Heal},
        };

        /**
         *  Switches the bodies of the casting player and the target player.
         */
        std::vector<Message>
        bodySwap(const Connection &client, const std::string &targetName);

        /**
         *  [Not yet implemented] Creates a decoy in combat so the player can flee
         */
        std::vector<Message>
        decoy(const Connection &client);

        /**
         *  Places target player under the effects of the Confuse spell, which causes
         *  the target player's next chat messages to be in Pig Latin.
         */
        std::vector<Message>
        confuse(const Connection &client, const std::string &targetName);

        /**
         *  Fully restores the target player's health. Can only be used when target is out of combat.
         */
        std::vector<Message>
        heal(const Connection &client, const std::string &targetName);


        /**
         *  Removes a body swap SpellInstance containing the specified player ID as the target or caster and reverts
         *  the swap between player clients.
         */
        void
        removeBodySwap(const model::ID &playerId);

        /**
         *  Removes a confuse SpellInstance containing the specified player ID as the target
         */
        void
        removeConfuse(const model::ID &playerId);

        /**
         *  Decrements the remaining duration of Confuse spell instances, erasing them if expired.
         */
        void
        processConfuseInstancesCycle(std::deque<Message> &messages);

        /**
         *  Decrements the remaining duration of Body Swap spell instances, reverting the swap and
         *  erasing them if expired.
         */
        void
        processBodySwapInstancesCycle(std::deque<Message> &messages);

    public:
        /**
         *  Constructs a MagicHandler instance with a pointer to the
         *  AccountHandler instance used by the Game class.
         */
        MagicHandler(AccountHandler &accountHandler, CombatHandler &combatHandler);

        /**
         *  Returns a formatted string describing available spells.
         */
        std::string
        getSpells();

        /**
         *  Casts a spell if the spell name is valid
         */
        std::vector<Message>
        castSpell(const Connection &client, const std::string &spellName, const std::string &targetName = "");

        /**
         *  Returns true if the client's Player is under the effects of the Body Swap spell
         */
        bool
        isBodySwapped(const Connection &client);

        /**
         *  Returns true if the client's Player is under the effects of the Confuse spell
         */
        bool
        isConfused(const Connection &client);

        /**
         *  Converts a string into Pig Latin
         */
        void
        confuseSpeech(std::string &message);

        /**
         *  Handles the active spells affecting the logged out Player.
         */
        void
        handleLogout(const Connection &client);

        /**
         *  Decrements the remaining duration of active spells and removing their
         *  effects on expiration. Creates Messages to notify players when spells
         *  have expired.
         */
        void
        processCycle(std::deque<Message> &messages);
    };
}

#endif //MAGICHANDLER_H
