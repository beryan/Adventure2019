//
// Created by arehal on 1/30/19.
//

#include "FileParser.h"
#include "json.hpp"


#include <boost/filesystem.hpp>

using json = nlohmann::json;


void ParseDataFile(std::string filePath){

    std::string extension = boost::filesystem::extension(filePath);
    if(extension == JSON_EXTENSION){
        parseJson(filePath);
    }

}

