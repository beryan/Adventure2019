//
// Created by jnhkm on 2019-02-23.
//

#include "Equipment.h"

using model::Equipment;

namespace model {
    Equipment::Equipment() {}

    std::map<int, Object> Equipment::getMappedEquipment() {
        return this->equipment;
    }

    std::vector<Object> Equipment::getVectorEquipment() const {
        std::vector<Object> container;
        container.reserve(equipment.size());

        for (auto const& [key, val] : equipment) {
            container.push_back(val);
        }

        return container;
    }

    void Equipment::mapEquipment(std::vector<Object> &items) {
        for (Object& item : items) {
            equipment.insert(std::pair<int, Object>(item.getSlot(), item));
        }
    }

    void Equipment::equipItem(Object object) {
        this->equipment.insert(std::pair<int, Object>(object.getSlot(), std::move(object)));
    }

    Object Equipment::unequipItem(Object &item) {
        Object temp;
        if (item.getId() == this->equipment.at(item.getSlot()).getId()) {
            temp = unequipSlot(item.getSlot());
        }

        return std::move(temp);
    }

    Object Equipment::unequipSlot(Slot slot) {
        Object temp = std::move(this->equipment.at(slot));
        this->equipment.erase(slot);

        return std::move(temp);
    }

    bool Equipment::isSlotOccupied(const Slot &slot) {
        return static_cast<bool>(equipment.count(slot));
    }

    bool Equipment::isItemEquipped(const Object &item) {
        return  isSlotOccupied(item.getSlot()) &&
                equipment.at(item.getSlot()).getId() == item.getId();
    }
}