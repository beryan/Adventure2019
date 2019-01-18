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
        inline int getId();

        inline void setId(int id);

        inline std::string getName();

        inline void setName(std::string name);

        inline std::string getDescription();

        inline void setDescription(std::string description);

    private:
        int id;

        std::string name;

        std::string description;
    };
}

#endif //WEBSOCKETNETWORKING_OBJECT_H