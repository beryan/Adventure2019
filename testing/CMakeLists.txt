enable_testing()


include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/
)

file(GLOB SOURCES "*.cpp")

add_executable(runAllTests
        lib/gtest_main.cc
        lib/gmock-gtest-all.cc
        ${SOURCES})

target_link_libraries(runAllTests
        model
        logic
        handler
        pthread
        ${CMAKE_THREAD_LIBS_INIT}
        )

add_test(NAME AllTests COMMAND runAllTests)
