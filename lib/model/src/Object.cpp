//
// Created by Waswa Olunga on 2019-01-17.
//

#include <Object.h>
#include <string>

using model::Object;

namespace model {

    void
    model::Object::setId(int id){
        this->id = id;
    }

    int
    model::Object::getId() {
        return this->id;
    }

    void
    model::Object::setName(std::string name){
        this->name = name;
    }

    std::string
    model::Object::getName(){
        return this->name;
    }

    void
    model::Object::setDescription(std::string description) {
        this->description = description;
    }

    std::string
    model::Object::getDescription(){
        return this->description;
    }
}