add_library(json INTERFACE)

target_sources(json
        INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/include/json.hpp")

find_package(Boost 1.68 COMPONENTS filesystem REQUIRED)

target_include_directories(json
        INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        )

add_library(fileparser
        src/DataManager.cpp)

target_include_directories(fileparser
        PRIVATE
        ${Boost_INCLUDE_DIR}
        )

target_link_libraries(fileparser
        PUBLIC
        model
        PRIVATE
        ${Boost_LIBRARIES}
        )