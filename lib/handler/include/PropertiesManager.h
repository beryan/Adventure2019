//
// Created by Stephen Wanhella on 2019-01-29.
//

#ifndef PROPERTIESMANAGER_H
#define PROPERTIESMANAGER_H

#ifndef TEST_MODE
#define TEST_MODE false
#endif

#include <fstream>
#include <iterator>
#include "json.hpp"
#include <iostream>
#include <vector>

using Json = nlohmann::json;
constexpr auto PROPERTIES_PATH = "lib/data/properties.json";
constexpr auto TEST_PROPERTIES_PATH = "lib/data/testProperties.json";

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
            std::ifstream ifs;

            if (TEST_MODE) {
                ifs = std::ifstream(TEST_PROPERTIES_PATH);

            } else {
                ifs = std::ifstream(PROPERTIES_PATH);
            }


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