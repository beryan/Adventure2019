//
// Created by jnhkm on 2019-02-23.
//

#include "Inventory.h"

using model::Inventory;

namespace model {
    Inventory::Inventory() {}

    std::unordered_map<model::ID, Object> Inventory::getMappedInventory() {
        return this->inventory;
    }

    std::vector<Object> Inventory::getVectorInventory() const {
        std::vector<Object> container;
        container.reserve(this->inventory.size());

        for (auto const& item : this->inventory) {
            container.push_back(item.second);
        }

        return container;
    }

    void Inventory::mapInventory(std::vector<Object> items) {
        this->inventory.clear();
        for (Object item : items) {
            this->inventory.insert(std::pair<model::ID, Object>(item.getId(), item));
        }
    }

    void Inventory::addItemToInventory(const Object &item) {
        this->inventory.insert(std::pair<model::ID, Object>(item.getId(), item));
    }

    Object Inventory::removeItemFromInventory(const Object &item) {
        Object temp = std::move(this->inventory.at(item.getId()));
        this->inventory.erase(item.getId());

        return std::move(temp);
    }

    bool Inventory::isItemInInventory(const Object &item) {
        return static_cast<bool>(this->inventory.count(item.getId()));
    }
}
