add_library(model
        src/Character.cpp
        src/Player.cpp
        src/NPC.cpp
        src/Object.cpp
        src/ExtraObjectInfo.cpp
        src/Game.cpp
        src/Room.cpp
        src/Area.cpp
        src/Reset.cpp
        src/World.cpp
        src/WorldHandler.cpp
        src/PlayerHandler.cpp
        src/PropertiesManager.cpp
        )

target_include_directories(model
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        )

target_link_libraries(model networking json fileparser)

set_target_properties(model
        PROPERTIES
        LINKER_LANGUAGE CXX
        CXX_STANDARD 17
        )

install(TARGETS model
        ARCHIVE DESTINATION lib
        )
