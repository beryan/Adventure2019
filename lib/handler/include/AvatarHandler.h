//
// Created by louis on 18/03/19.
//

#ifndef AVATARHANDLER_H
#define AVATARHANDLER_H

#include <string>
#include <map>
#include "Server.h"
#include "AccountHandler.h"

using networking::Connection;
using handler::AccountHandler;

namespace handler {
    /**
     * Provides an interface for creating a Player's avatar.
     */
    class AvatarHandler {
    private:
        enum class AvatarCreationStage{Gender, Race, Trait1, Trait2, Confirm};

        AccountHandler& accountHandler;
        std::map<Connection, AvatarCreationStage> creatingClients;
        std::map<Connection, Avatar> avatarsInCreation;

        std::string
        processGender(const Connection &client, const std::string &input);

        std::string
        processRace(const Connection &client, const std::string &input);

        std::string
        processFirstTrait(const Connection &client, const std::string &input);

        std::string
        processSecondTrait(const Connection &client, const std::string &input);

        std::string
        processConfirm(const Connection &client, const std::string &input);

    public:
        AvatarHandler(AccountHandler &accountHandler);

        bool
        isCreatingAvatar(const Connection &client) const;

        std::string
        processCreation(const Connection &client, const std::string &input = "");

        void
        exitCreation(const Connection &client);
    };
}


#endif //AVATARHANDLER_H
