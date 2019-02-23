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
        std::map<model::ID, Object> inventory;
    public:
        Inventory();

        std::map<model::ID, Object> getMappedInventory();

        std::vector<Object> getInventory() const;

        void addItemToInventory(Object object);

        Object removeItemFromInventory(Object object);

        void mapInventory(std::vector<Object> &items);
    };
}


#endif //INVENTORY_H
