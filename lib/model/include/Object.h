//
// Created by Waswa Olunga on 2019-01-17.
//

#ifndef WEBSOCKETNETWORKING_OBJECT_H
#define WEBSOCKETNETWORKING_OBJECT_H

#include <string>

namespace model {

/**
 *  @class Object
 *
 *  @brief A class describing an object.
 *
 *  The Object class contains all the properties of
 *  an object. These properties can be
 *  accessed and changed.
 */

    class Object {
    public:
        Object(int id, std::string name, std::string description)
            : id(id),
              name(std::move(name)),
              description(std::move(description)) {}

        int getId();

        void setId(int id);

        std::string getName();

        void setName(std::string name);

        std::string getDescription();

        void setDescription(std::string description);

        bool operator==(const Object& rhs) const;

    private:
        int id;

        std::string name;

        std::string description;
    };
}

#endif //WEBSOCKETNETWORKING_OBJECT_H