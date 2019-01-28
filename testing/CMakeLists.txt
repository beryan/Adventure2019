enable_testing()

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/
)

add_executable(runAllTests
        user-test.cpp
        playerHandler-test.cpp
        lib/gtest_main.cc
        lib/gmock-gtest-all.cc
        playerHandler-test.cpp)

target_link_libraries(runAllTests
        model
        pthread
        ${CMAKE_THREAD_LIBS_INIT}
        )

add_test(NAME AllTests COMMAND runAllTests)

