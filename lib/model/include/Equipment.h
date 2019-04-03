//
// Created by jnhkm on 2019-02-23.
//

#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include "Object.h"

using model::Object;
using model::Slot;

namespace model {
    class Equipment {
    private:
        constexpr static int WEAPON_OFFENCE_VALUE = 5;

        constexpr static int HEAD_ARMOUR_VALUE = 2;
        constexpr static int SHOULDERS_ARMOUR_VALUE = 1;
        constexpr static int CHEST_ARMOUR_VALUE = 3;
        constexpr static int HANDS_ARMOUR_VALUE = 1;
        constexpr static int LEGS_ARMOUR_VALUE = 2;

        constexpr static double WEAPON_CRITICAL_VALUE = 0.02;
        constexpr static double BACK_CRITICAL_VALUE = 0.03;

        constexpr static double BACK_DODGE_VALUE = 0.02;
        constexpr static double FEET_DODGE_VALUE = 0.03;

        /**
         * int: Numbers indicate which slot the item is in
         * Object: Item that is equipped
         */
        std::unordered_map<Slot, Object> equipment;

        friend std::ostream&operator<<(std::ostream& os, const Equipment& inv);
    public:
        Equipment();

        /**
         * @return the immutable unordered map of Equipment
         */
        std::unordered_map<Slot, Object> getMappedEquipment();

        /**
         * @return the immutable vector form of Equipment
         */
        std::vector<Object> getVectorEquipment() const;

        /**
         * Maps the list of items into Equipment
         * @param items: Items to be mapped
         */
        void mapEquipment(const std::vector<Object> &items);

        /**
         * Equips an item
         * @param object: Item to be equipped
         */
        void equipItem(const Object &object);

        /**
         * Uneqiups an item
         * @param item: Item to be unequipped
         * @return Ownership of the item that was unequipped
         */
        Object unequipItem(const Object &item);

        /**
         * Unequips a specific slot in Equipment
         * @param slot: Slot to be unequipped
         * @return Returns the ownership of the item that existed in the slot that was
         * unequipped
         */
        Object unequipSlot(const Slot &slot);

        /**
         * Checks to see if the Slot is currently occupied by an item
         * @param slot: Slot to be checked
         * @return True if an item exists in the slot, False if not
         */
        bool isSlotOccupied(const Slot &slot);

        /**
         * Checks to see if a specific item is equipped
         * @param item: Item to be checked
         * @return Returns True if the item is currently equipped, False if not
         */
        bool isItemEquipped(const Object &item);

        /**
         * @return Returns true if the Equipment is empty
         */
        bool isEquipmentEmpty();

        /**
         * @return Returns integer value based on offence modifier of each piece of equipment
         */
        int getOffenceValue();

        /**
         * @return Returns integer value based on defence modifier of each piece of equipment
         */
        int getDefenceValue();

        /**
         * @return Returns float value based on critical chance modifier of each piece of equipment
         */
        float getCriticalValue();

        /**
         * @return Returns float value based on dodge chance modifier of each piece of equipment
         */
        float getDodgeValue();
    };

    inline void to_json(json& equipmentJson, const Equipment &equipment) {
            equipmentJson = {
                    {"objects", equipment.getVectorEquipment()}
            };
    }

    inline void from_json(const json &equipmentJson, Equipment &equipment) {
            equipment.mapEquipment(equipmentJson.at("objects").get<std::vector<Object>>());
    }

}



#endif //EQUIPMENT_H
