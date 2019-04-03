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

using Json = nlohmann::json;
constexpr auto PROPERTIES_PATH = "lib/data/properties.json";

namespace handler {
    class PropertiesManager {
    public:
        constexpr static auto RESET_INTERVAL_PROPERTY_NAME = "reset_interval";

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

            Json json = Json::parse(ifs);

            bool wasFound = false;
            auto iterator = json.find(propertyName);
            if (iterator != json.end()) {
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