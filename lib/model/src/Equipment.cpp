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

    std::ostream&operator<<(std::ostream& os, const Equipment& rhs) {
        if (!rhs.equipment.empty()) {
            os << "\nEquipment:\n";
            os << "----------\n";
            for (const auto &kv : rhs.equipment) {
                auto slot = model::getStringFromSlot(kv.first);
                slot[0] = toupper(slot[0]);
                os << slot << ": [" << kv.second.getShortDescription() << "]\n";
            }
        } else {
            os << "Nothing equipped!\n";
        }

        return os;
    }
}