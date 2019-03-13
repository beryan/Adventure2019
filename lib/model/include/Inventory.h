//
// Created by jnhkm on 2019-02-23.
//

#ifndef INVENTORY_H
#define INVENTORY_H

#include "Object.h"

using model::Object;

namespace model {
    class Inventory {
    private:
        /**
         * Object: Object being stored
         * int: Quantity of the object
         */
        std::map<Object, int> inventory;

        friend std::ostream&operator<<(std::ostream& os, const Inventory& inv);
    public:
        Inventory();

        /**
         * @return The immutable unordered map of Inventory
         */
        std::map<Object, int> getMappedInventory();

        /**
         * @return The immutable vector form of Inventory
         */
        std::vector<Object> getVectorInventory();

        /**
         * Maps a list of items into the Inventory
         * @param items: Items to be mapped
         */
        void mapInventory(const std::vector<Object> &items);

        /**
         * Adds an item to the Inventory
         * @param item: Object to be added to the Inventory
         */
        void addItemToInventory(const Object &item);

        /**
         * Removes an item from the inventory which is synonymous
         * with the higher-level method of droppingItem
         * @param item: Item to be dropped from Inventory
         * @return The ownership of the item that was dropped
         */
        Object removeItemFromInventory(const Object &item);

        /**
         * Checks if the item exists in Inventory
         * @param item: Item to be found
         * @return True if the item was found, False if not
         */
        bool isItemInInventory(const Object &item);


        /**
         * @return True if the inventory is empty
         */
        bool isInventoryEmpty();
    };
}


#endif //INVENTORY_H
