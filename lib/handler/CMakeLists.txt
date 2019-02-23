add_library(handler
        src/PlayerAction.cpp
        src/CharacterHandler.cpp
        )

target_include_directories(handler
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        )

target_link_libraries(handler logic)

set_target_properties(handler
        PROPERTIES
        LINKER_LANGUAGE CXX
        CXX_STANDARD 17
        )

install(TARGETS handler
        ARCHIVE DESTINATION lib
        )
