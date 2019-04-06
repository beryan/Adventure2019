//
// Created by Brittany Ryan on 2019-04-02.
//

#include "Command.h"

using game::Command;
using model::Role;

namespace game {
    bool isInvalidRole(const Command &command, const model::Role &role) {
        bool isBuildCommand = std::find(build.begin(), build.end(), command) != build.end();
        bool isAdminCommand = std::find(admin.begin(), admin.end(), command) != admin.end();

        return (isBuildCommand && role == Role::Default) || (isAdminCommand && role != Role::Admin);
    }

    bool isInvalidFormat(const Command &command, const std::string &parameters) {
        bool wrongFormat = ((command == Command::Tell || command == Command::Give)
                            && parameters.find(' ') == std::string::npos);
        bool isParamCommand = std::find(param.begin(), param.end(), command) != param.end();
        return (wrongFormat || (isParamCommand && parameters.empty()));
    }

    bool isRoleCommand(const Command &command) {
        bool isBuildCommand = std::find(build.begin(), build.end(), command) != build.end();
        bool isAdminCommand = std::find(admin.begin(), admin.end(), command) != admin.end();

        return isBuildCommand || isAdminCommand;
    };
}
