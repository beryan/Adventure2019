//
// Created by Stephen Wanhella on 2019-01-19.
//

#include "User.h"
#include <algorithm>

using model::User;

namespace model_utils {
    /**
   *  @brief called to pickup item which handles swapping if item slot is occupied
   */
    void equip(std::array<Item*, Slot::Count> &equipment, std::vector<Item> &inventory, Item *item) {
        if (Item *var = equipment.at(item->getSlot()) ; var != nullptr) {
            std::swap(*var, *item);
        }
        else {
            *var = std::move(*item);
        }

        inventory.erase(std::find(inventory.begin(), inventory.end(), *item));
    }

    void unequip(std::array<Item*, Slot::Count> &equipment, std::vector<Item> &inventory, Slot slot) {
        if (Item *var = equipment.at(slot); var != nullptr) {
            inventory.push_back(std::move(*var));
        }
    }

    void pickup(std::vector<Item> &inventory, Item item) {
        inventory.push_back(std::move(item));
    }

    // drops the item from the inventory (index starts from 1)
    Item drop(std::vector<Item> &inventory, int index) {
        Item item = std::move(inventory.at(index));
        inventory.erase(inventory.begin() + index);
        return std::move(item);
    }

    Item* drop(std::array<Item*, Slot::Count> equipment, int index) {
        Item *item = std::move(equipment.at(index));
        equipment[index] = nullptr;
        return std::move(item);
    }
}

namespace model {
    User::User(int id) :
        id(id),
        keywords({}),
        description({}),
        inventory({}),
        equipment({}),
        health(STARTING_HEALTH)
        {}

    User::User(int id, std::vector<std::string> keys, std::vector<std::string> desc) :
        id(id),
        keywords(std::move(keys)),
        description(std::move(desc)),
        inventory({}),
        equipment({}),
        health(STARTING_HEALTH)
        {}

    int User::getId() {
      return id;
    }

    void User::setId(int id) {
      this->id = id;
    }

    void User::addKeyword(std::string key) {
      keywords.push_back(std::move(key));
    }

    void User::removeKeyword(int index) {
      keywords.erase(keywords.begin() + index);
    }

    std::vector<std::string> User::getKeywords() {
      return keywords;
    }

    double User::getHealth() {
      return health;
    }

    void User::setHealth(double health) {
      this->health = health;
    }

    std::vector<std::string> User::getDescription() {
      return description;
    }

    void User::setDescription(std::string desc) {
      description.push_back(std::move(desc));
    }

    bool User::operator==(const User& user) {
      return id == user.id;
    }

    std::array<Item*, Slot::Count> User::getEquipments() {
        return equipment;
    }

    void User::equipItem(Item *item) {
        if (item->getCanEquip()) {
            model_utils::equip(equipment, inventory, item);
        }
    }

    std::vector<Item> User::getInventory() {
        return inventory;
    }

    void User::addItem(Item item) {
        model_utils::pickup(inventory, item);
    }
}

