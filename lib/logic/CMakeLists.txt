file(GLOB SOURCES "src/*.cpp")

add_library(logic
        ${SOURCES}
        )

target_include_directories(logic
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        )

target_link_libraries(logic model)

set_target_properties(model
        PROPERTIES
        LINKER_LANGUAGE CXX
        CXX_STANDARD 17
        )

install(TARGETS logic
        ARCHIVE DESTINATION lib
        )

set_target_properties(logic
        PROPERTIES
        COMPILE_FLAGS "${CXX_COVERAGE_FLAGS}"
        )