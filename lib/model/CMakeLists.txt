add_library(model
        src/User.cpp
        src/Player.cpp
        src/NPC.cpp
        src/Object.cpp
        src/Item.cpp
        include/Role.h
        src/User.cpp
        src/Door.cpp
        src/Room.cpp
        src/Area.cpp
        src/Reset.cpp
        src/World.cpp
        src/WorldHandler.cpp
        src/PlayerHandler.cpp)

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

target_link_libraries(model json)

install(TARGETS model
        ARCHIVE DESTINATION lib
        )
