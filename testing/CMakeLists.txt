find_package(Boost REQUIRED COMPONENTS unit_test_framework REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_executable(unit_tests
        player_class_test.cpp
        )

target_link_libraries(unit_tests
        model
        )

