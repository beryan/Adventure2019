add_subdirectory(lib/googlemock)
enable_testing()

add_executable(runAllTests
        user-test.cpp
        )

target_link_libraries(runAllTests gmock gtest gtest_main)
target_link_libraries(runAllTests model)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/googletest/include/
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/googlemock/include/
)

add_test(NAME AllTests COMMAND runAllTests)

