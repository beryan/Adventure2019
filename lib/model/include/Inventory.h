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
        std::unordered_map<model::ID, Object> inventory;
    public:
        Inventory();

        std::unordered_map<model::ID, Object> getMappedInventory();

        std::vector<Object> getVectorInventory() const;

        void addItemToInventory(Object object);

        Object removeItemFromInventory(Object object);

        void mapInventory(std::vector<Object> &items);

        bool isItemInInventory(const Object &item);
    };
}


#endif //INVENTORY_H
