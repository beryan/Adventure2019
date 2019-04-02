//
// Created by jnhkm on 2019-01-18.
//

#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <array>
#include <map>
#include "Avatar.h"
#include "Character.h"
#include "Inventory.h"
#include "Equipment.h"

using model::Avatar;
using model::Inventory;
using model::Equipment;

namespace model {
    /**
    *  @class Player
    *
    *  @brief A struct describing a player.
    *
    *  The Player class describes all the attributes of
    *  a specific type of character in the application.
    */

    class Player : public Character {
    public:
        Player();

        Player(model::ID id, std::string_view username, std::string_view password);

        Player(model::ID id, std::string_view username, std::string_view password, const model::ID &roomID);

        std::string getUsername() const;

        void setUsername(std::string_view username);

        std::string getPassword() const;

        void setPassword(std::string_view password);

        Avatar getAvatar() const;

        std::string getDescription() const;

        void setAvatar(Avatar avatar);

        bool operator==(const Player& player) const;

        /************ Inventory ************/

        Inventory &getInventory();

        Equipment &getEquipment();

        /************ ROOM ************/

        model::ID getCurrRoomID();

        void setCurrRoomID(const model::ID &id);

//        static constexpr model::ID STARTING_LOCATION = 1000;
        static constexpr model::ID STARTING_LOCATION = 8800;

    private:
        std::string username;

        std::string password;

        Avatar avatar;

        Inventory inventory;

        Equipment equipment;

        model::ID currRoomID;
    };

    inline void from_json(const json &j, Player &p) {
        p.setId(j.at("id").get<model::ID>());
        p.setUsername(j.at("username").get<std::string>());
        p.setPassword(j.at("password").get<std::string>());
        if(j.find("avatar") != j.end()) {
            p.setAvatar(j.at("avatar").get<Avatar>());
        }
    }

    inline void to_json(json& j, const Player &p){
        if(p.getAvatar().isDefined()){
            j = {
                    {"id", p.getId()},
                    {"username", p.getUsername()},
                    {"password", p.getPassword()},
                    {"avatar", p.getAvatar()}
            };
        }else {
            j = {
                    {"id", p.getId()},
                    {"username", p.getUsername()},
                    {"password", p.getPassword()}
            };
        }
    }
}

#endif //PLAYER_H
