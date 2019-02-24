//
// Created by jnhkm on 2019-02-23.
//

#include "Inventory.h"

using model::Inventory;

namespace model {
    Inventory::Inventory() {}

    std::map<model::ID, Object> Inventory::getMappedInventory() {
        return this->inventory;
    }

    std::vector<Object> Inventory::getInventory() const {
        std::vector<Object> container;
        container.reserve(inventory.size());

        for (auto const& [key, val] : inventory) {
            container.push_back(val);
        }

        return container;
    }

    void Inventory::addItemToInventory(Object object) {
        this->inventory.insert(std::pair<model::ID, Object>(object.getId(), std::move(object)));
    }

    Object Inventory::removeItemFromInventory(Object object) {
        Object temp = std::move(this->inventory.at(object.getId()));
        this->inventory.erase(object.getId());

        return std::move(temp);
    }

    void Inventory::mapInventory(std::vector<Object> &items) {
        for (Object& item : items) {
            inventory.insert(std::pair<model::ID, Object>(item.getId(), item));
        }
    }

    bool Inventory::isItemInInventory(const Object &item) {
        return static_cast<bool>(inventory.count(item.getId()));
    }
}
