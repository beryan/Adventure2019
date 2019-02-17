#ifndef MAGICHANDLER_H
#define MAGICHANDLER_H

#include "PlayerHandler.h"

using model::Player;

namespace model {
    class MagicHandler {
    private:
        PlayerHandler* playerHandler;
        std::vector<std::pair<std::pair<std::string, std::string>, unsigned int>> swapTracker;

        enum class Spell {
            Swap,
            Decoy,
            Confuse
        };

        std::map<std::string, Spell> spellMap = {
            {"swap", Spell::Swap},
            {"decoy", Spell::Decoy},
            {"confuse", Spell::Confuse},
        };

        std::map<Spell, std::vector<std::string>> spellWordsMap = {
            {Spell::Swap,    {"swap"}},
            {Spell::Decoy,   {"decoy"}},
            {Spell::Confuse, {"confuse"}}
        };

        std::vector <Message>
        swap(const Connection &client, const std::string &targetName);

        std::vector <Message>
        decoy(const Connection &client);

        std::vector<Message>
        confuse(const Connection &client, const std::string &targetName);

    public:
        explicit MagicHandler(PlayerHandler* playerHandler);

        std::vector<Message>
        castSpell(const Connection &client, const std::string &param, const std::string &targetName = "");

        void
        handleLogout(const Connection &client);

        void
        processCycle(std::deque<Message> &messages);
    };
}

#endif //MAGICHANDLER_H
