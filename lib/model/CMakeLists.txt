file(GLOB SOURCES "src/*.cpp")

add_library(model
        ${SOURCES}
        )

#[[find_package(Boost 1.68 COMPONENTS filesystem REQUIRED)]]

target_include_directories(model
        PUBLIC
            $<INSTALL_INTERFACE:include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            ${Boost_INCLUDE_DIR}
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
        )

target_link_libraries(model networking json fileparser ${Boost_LIBRARIES})

set_target_properties(model
        PROPERTIES
        LINKER_LANGUAGE CXX
        CXX_STANDARD 17
        )

install(TARGETS model
        ARCHIVE DESTINATION lib
        )

set_target_properties(model
        PROPERTIES
        COMPILE_FLAGS "${CXX_COVERAGE_FLAGS}"
        )