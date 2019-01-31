//
// Created by Waswa Olunga on 2019-01-18.
//

#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include "Types.h"

namespace model {

    /**
     *  @class Object
     *
     *  @brief A class describing an object.
     *
     *  The Object class contains all the properties of
     *  an object that is part of the inventory of a player
     *  or an object that is part of a room.
     */

    // Count returns the length of the Slots
    enum Slot {Head, Body, Legs, Arm, Weapon, Misc, Count};

    class Object {
    public:
        Object();

        Object(model::ID id, std::string name, std::string description, Slot slot);

        Object(model::ID id, std::string name, std::string description, bool canEquip, Slot slot);

        model::ID getId() const;

        void setId(model::ID id);

        std::string getName() const;

        void setName(std::string name);

        std::string getDescription() const;

        void setDescription(std::string description);

        Slot getSlot() const;

        void setSlot(Slot slot);

        bool canBeEquipped() const;

        bool operator==(const Object& NPC) const;

        static constexpr model::ID DEFAULT_ID = -1;

    private:
        model::ID id;

        std::string name;

        std::string description;

        Slot slot;
    };
}

#endif //OBJECT_H