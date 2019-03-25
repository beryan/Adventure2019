//
// Created by jnhkm on 2019-02-23.
//

#include "Equipment.h"

using model::Equipment;

namespace model {
    Equipment::Equipment() {}

    std::unordered_map<Slot, Object> Equipment::getMappedEquipment() {
        return this->equipment;
    }

    std::vector<Object> Equipment::getVectorEquipment() const {
        std::vector<Object> container;
        container.reserve(this->equipment.size());

        for (auto const& item : this->equipment) {
            container.push_back(item.second);
        }

        return container;
    }

    void Equipment::mapEquipment(const std::vector<Object> &items) {
        this->equipment.clear();
        for (Object item : items) {
            this->equipment.insert(std::pair<Slot, Object>(item.getSlot(), item));
        }
    }

    void Equipment::equipItem(const Object &object) {
        this->equipment.insert(std::pair<Slot, Object>(object.getSlot(), object));
    }

    Object Equipment::unequipItem(const Object &item) {
        Object temp;
        if (item == this->equipment.at(item.getSlot())) {
            temp = unequipSlot(item.getSlot());
        }

        return temp;
    }

    Object Equipment::unequipSlot(const Slot &slot) {
        Object temp = std::move(this->equipment.at(slot));
        this->equipment.erase(slot);

        return temp;
    }

    bool Equipment::isSlotOccupied(const Slot &slot) {
        return static_cast<bool>(this->equipment.count(slot));
    }

    bool Equipment::isItemEquipped(const Object &item) {
        return  isSlotOccupied(item.getSlot()) &&
                this->equipment.at(item.getSlot()) == item;
    }

    bool Equipment::isEquipmentEmpty() {
        return this->equipment.empty();
    }

    int Equipment::getOffenceValue() {
        int offenceValue = 0;

        if (this->isSlotOccupied(Slot::Weapon)) {
            offenceValue += WEAPON_OFFENCE_VALUE;
        }

        return offenceValue;
    }

    int Equipment::getDefenceValue() {
        int defenceValue = 0;

        for (const auto &gear : this->getVectorEquipment()) {
            switch (gear.getSlot()) {
                case Slot::Head:
                    defenceValue += HEAD_ARMOUR_VALUE;
                    break;

                case Slot::Shoulders:
                    defenceValue += SHOULDERS_ARMOUR_VALUE;
                    break;

                case Slot::Chest:
                    defenceValue += CHEST_ARMOUR_VALUE;
                    break;

                case Slot::Hands:
                    defenceValue += HANDS_ARMOUR_VALUE;
                    break;

                case Slot::Legs:
                    defenceValue += LEGS_ARMOUR_VALUE;
                    break;

                default:
                    break;
            }
        }

        return defenceValue;
    }

    float Equipment::getCriticalValue() {
        float criticalValue = 0;

        if (this->isSlotOccupied(Slot::Weapon)) {
            criticalValue += WEAPON_CRITICAL_VALUE;
        }

        if (this->isSlotOccupied(Slot::Back)) {
            criticalValue += BACK_CRITICAL_VALUE;
        }

        return criticalValue;
    }

    float Equipment::getDodgeValue() {
        float dodgeValue = 0;

        if (this->isSlotOccupied(Slot::Back)) {
            dodgeValue += FEET_DODGE_VALUE;
        }

        if (this->isSlotOccupied(Slot::Feet)) {
            dodgeValue += FEET_DODGE_VALUE;
        }

        return dodgeValue;
    }

    std::ostream&operator<<(std::ostream& os, const Equipment& rhs) {
        if (!rhs.equipment.empty()) {
            os << "\nEquipment:\n";
            os << "----------\n";
            for (const auto &kv : rhs.equipment) {
                os << kv.first << ". [" << kv.second.getShortDescription() << "]";

                switch (kv.second.getSlot()) {
                    case Slot::Weapon:
                        os << " (+" << Equipment::WEAPON_OFFENCE_VALUE << " Attack,"
                           << " +" << Equipment::WEAPON_CRITICAL_VALUE * 100 << "% Critical Chance)";
                        break;

                    case Slot::Head:
                        os << " (+" << Equipment::HEAD_ARMOUR_VALUE << " Armour)";
                        break;

                    case Slot::Shoulders:
                        os << " (+" << Equipment::SHOULDERS_ARMOUR_VALUE << " Armour)";
                        break;

                    case Slot::Chest:
                        os << " (+" << Equipment::CHEST_ARMOUR_VALUE << " Armour)";
                        break;

                    case Slot::Back:
                        os << " (+" << Equipment::BACK_CRITICAL_VALUE * 100 << "% Critical Chance,"
                           << " +" << Equipment::BACK_DODGE_VALUE * 100 <<  "% Dodge Chance)";
                        break;

                    case Slot::Hands:
                        os << " (+" << Equipment::HANDS_ARMOUR_VALUE << " Armour)";
                        break;

                    case Slot::Legs:
                        os << " (+" << Equipment::LEGS_ARMOUR_VALUE << " Armour)";
                        break;

                    case Slot::Feet:
                        os << " (+" << Equipment::FEET_DODGE_VALUE * 100 << "% Dodge Chance)";
                        break;

                    default:
                        break;
                }

                os << "\n";
            }
        } else {
            os << "Nothing equipped!\n";
        }

        return os;
    }
}