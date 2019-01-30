//
// Created by Stephen Wanhella on 2019-01-29.
//

#ifndef PROPERTIESMANAGER_H
#define PROPERTIESMANAGER_H

#include <string>


namespace model {
  class PropertiesManager {
  public:
    /**
     *
     * @param propertyName the name of the property to look for
     * @param result variable to store the result
     * @return true if the item exists and was stored in result, false otherwise
     */
    static bool getStringProperty(std::string propertyName, std::string& result);

    /**
     *
     * @param propertyName the name of the property to look for
     * @param result variable to store the result
     * @return true if the item exists and was stored in result, false otherwise
     */
    static bool getIntProperty(std::string propertyName, int& result);

    /**
     *
     * @param propertyName the name of the property to look for
     * @param result variable to store the result
     * @return true if the item exists and was stored in result, false otherwise
     */
    static bool getBoolProperty(std::string propertyName, bool& result);

  private:

    // class can not be instantiated
    PropertiesManager() = default;

  };

}


#endif PROPERTIESMANAGER_H