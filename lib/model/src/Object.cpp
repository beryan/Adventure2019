//
// Created by Waswa Olunga on 2019-01-17.
//

#include <Object.h>
#include <string>

using model::Object;

namespace model {

    int
    Object::getId() {
        return this->id;
    }

    void
    Object::setId(int id){
        this->id = id;
    }

    std::string
    Object::getName(){
        return this->name;
    }

    void
    Object::setName(std::string name){
        this->name = std::move(name);
    }

    std::string
    Object::getDescription(){
        return this->description;
    }

    void
    Object::setDescription(std::string description) {
        this->description = std::move(description);
    }
}