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

    public:
        AvatarHandler(AccountHandler &accountHandler);

        bool
        isCreatingAvatar(const Connection &client);

        std::string
        processCreation(const Connection &client, const std::string &input = "");

        void
        exitCreation(const Connection &client);
    };
}


#endif //AVATARHANDLER_H
