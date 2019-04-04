enable_testing()


include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/
)

file(GLOB SOURCES "*.cpp")

add_definitions(-DtestMode=true)

add_executable(runAllTests
        lib/gtest_main.cc
        lib/gmock-gtest-all.cc
        ${SOURCES})

target_link_libraries(runAllTests
        model
        logic
        handler
        game
        pthread
        ${CMAKE_THREAD_LIBS_INIT}
        )

add_test(NAME AllTests COMMAND runAllTests)

set_target_properties(runAllTests
        PROPERTIES
        LINK_FLAGS "${CXX_COVERAGE_FLAGS}"
        )