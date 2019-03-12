file(GLOB SOURCES "src/*.cpp")

add_library(game
        ${SOURCES}
        )

target_include_directories(game
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        )

target_link_libraries(game networking model handler)

set_target_properties(game
        PROPERTIES
        LINKER_LANGUAGE CXX
        CXX_STANDARD 17
        )

install(TARGETS game
        ARCHIVE DESTINATION lib
        )

set_target_properties(game
        PROPERTIES
        COMPILE_FLAGS "${CXX_COVERAGE_FLAGS}"
        )