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
        Inventory getInventoryConst() const;
        void setInventory(Inventory inventory);

        Equipment &getEquipment();
        Equipment getEquipmentConst() const;
        void setEquipment(Equipment equipment);

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
        p.setRole(Character::stringToRoleMap.at(j.at("role").get<std::string>()));
        if(j.find("avatar") != j.end()) {
            p.setAvatar(j.at("avatar").get<Avatar>());
        }
        if(j.find("equipment") != j.end()) {
            p.setEquipment(j.at("equipment").get<Equipment>());
        }
        if(j.find("inventory") != j.end()) {
            p.setInventory(j.at("inventory").get<Inventory>());
        }
    }

    inline void to_json(json& j, const Player &p){
        j = {
                {"id", p.getId()},
                {"username", p.getUsername()},
                {"password", p.getPassword()},
                {"role", Character::roleToStringMap.at(p.getRole())}
        };

        if(p.getAvatar().isDefined()){
            j.push_back({"avatar", p.getAvatar()});
        };

        if(p.getEquipmentConst().getVectorEquipment().size() > 0){
            j.push_back({"equipment", p.getEquipmentConst()});
        }

        if(p.getInventoryConst().getVectorInventory().size() > 0){
            j.push_back({"inventory", p.getInventoryConst()});
        }
    }
}

#endif //PLAYER_H
