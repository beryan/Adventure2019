//
// Created by jnhkm on 2019-02-23.
//

#include "Equipment.h"

using model::Equipment;

namespace model {
    Equipment::Equipment() {}

    std::map<int, Object> Equipment::getMappedEquipments() {
        return this->equipments;
    }

    std::vector<Object> Equipment::getEquipments() const {
        std::vector<Object> container;
        container.reserve(equipments.size());

        for (auto const& [key, val] : equipments) {
            container.push_back(val);
        }

        return container;
    }

    void Equipment::mapEquipments(std::vector<Object> &items) {
        for (Object& item : items) {
            equipments.insert(std::pair<int, Object>(item.getSlot(), item));
        }
    }

    void Equipment::equipItem(Object object) {
        this->equipments.insert(std::pair<int, Object>(object.getSlot(), std::move(object)));
    }

    Object Equipment::unequipSlot(Slot slot) {
        Object temp = std::move(this->equipments.at(slot));
        this->equipments.erase(slot);

        return std::move(temp);
    }
}