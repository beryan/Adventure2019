//
// Created by Stephen Wanhella on 2019-01-29.
//

#ifndef PROPERTIESMANAGER_H
#define PROPERTIESMANAGER_H


#include <fstream>
#include <iterator>
#include "json.hpp"
#include <iostream>
#include <vector>

using json = nlohmann::json;
const std::string PROPERTIES_PATH = "lib/data/properties.json";

namespace model {
    class PropertiesManager {
    public:
        /**
         *
         * @tparam T type of object to look for
         * @param propertyName the name of the property to look for
         * @param result variable to store the result
         * @return true if the item exists and was stored in result, false otherwise
         */
        template<class T>
        static bool getProperty(std::string propertyName, T &result) {
            std::ifstream ifs(PROPERTIES_PATH);

            json t = json::parse(ifs);

            bool wasFound = false;
            auto iterator = t.find(propertyName);
            if (iterator != t.end()) {
                result = iterator->get<T>();
                wasFound = true;
            }

            return wasFound;
        }

    private:
        // class can not be instantiated
        PropertiesManager() = default;
    };

}


#endif //PROPERTIESMANAGER_H