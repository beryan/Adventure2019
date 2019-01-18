add_library(model
        src/User.cpp
        src/UserCollection.cpp
        src/Object.cpp
        src/Item.cpp
        )

target_include_directories(model
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        )

set_target_properties(model
        PROPERTIES
        LINKER_LANGUAGE CXX
        CXX_STANDARD 17
        )

install(TARGETS model
        ARCHIVE DESTINATION lib
        )