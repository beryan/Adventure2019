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

        Inventory &getMutableInventory();
        Inventory getImmutableInventory() const;
        void setInventory(const Inventory &inventory);

        Equipment &getMutableEquipment();
        Equipment getImmutableEquipment() const;
        void setEquipment(const Equipment &equipment);

        /************ ROOM ************/

        model::ID getCurrRoomID();

        void setCurrRoomID(const model::ID &id);

        static constexpr model::ID STARTING_LOCATION = 8800;

    private:
        std::string username;

        std::string password;

        Avatar avatar;

        Inventory inventory;

        Equipment equipment;

        model::ID currRoomID;
    };

    inline void from_json(const Json &json, Player &player) {
        player.setId(json.at("id").get<model::ID>());
        player.setUsername(json.at("username").get<std::string>());
        player.setPassword(json.at("password").get<std::string>());
        player.setRole(Character::stringToRoleMap.at(json.at("role").get<std::string>()));
        if(json.find("avatar") != json.end()) {
            player.setAvatar(json.at("avatar").get<Avatar>());
        }
        if(json.find("equipment") != json.end()) {
            player.setEquipment(json.at("equipment").get<Equipment>());
        }
        if(json.find("inventory") != json.end()) {
            player.setInventory(json.at("inventory").get<Inventory>());
        }
    }

    inline void to_json(Json& json, const Player &player){
        json = {
                {"id", player.getId()},
                {"username", player.getUsername()},
                {"password", player.getPassword()},
                {"role", Character::roleToStringMap.at(player.getRole())}
        };

        if(player.getAvatar().isDefined()){
            json.push_back({"avatar", player.getAvatar()});
        };

        if(player.getImmutableEquipment().getVectorEquipment().size() > 0){
            json.push_back({"equipment", player.getImmutableEquipment()});
        }

        if(player.getImmutableInventory().getVectorInventory().size() > 0){
            json.push_back({"inventory", player.getImmutableInventory()});
        }
    }
}

#endif //PLAYER_H
