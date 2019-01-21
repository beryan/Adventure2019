add_library(model
        src/Player.cpp
        src/NPC.cpp
        src/UserCollection.cpp
        src/Object.cpp
        src/Item.cpp
        src/Door.cpp
        src/Game.cpp
        src/ActionResult.cpp
        src/Room.cpp
        src/World.cpp
        src/WorldHandler.cpp
        )

target_include_directories(model
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        )

target_link_libraries(model networking json)

set_target_properties(model
        PROPERTIES
        LINKER_LANGUAGE CXX
        CXX_STANDARD 17
        )

install(TARGETS model
        ARCHIVE DESTINATION lib
        )
