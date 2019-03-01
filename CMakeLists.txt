cmake_minimum_required(VERSION 3.12)
project(WebSocketNetworking)

find_package(Boost 1.68 COMPONENTS filesystem REQUIRED)

if (ENABLE_COVERAGE)
    # NOTE: Coverage only works/makes sense with debug builds
    set(CMAKE_BUILD_TYPE "Debug")

    if("${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
        set(CXX_COVERAGE_FLAGS "-fprofile-instr-generate -fcoverage-mapping")
    else()
        message(FATAL_ERROR "Coverage generation is only supported for clang")
    endif()
    message("Enabling coverage instrumentation with:\n ${CXX_COVERAGE_FLAGS}")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PACKAGE_NAME WebSocketNetworking)
set(PACKAGE_VERSION 0.1)
set(PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION}")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fno-rtti")
#[[
set(CMAKE_CXX_FLAGS_DEBUG
    "-O0 -g -fsanitize=address,undefined -fno-omit-frame-pointer"
)
]]

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

# And the project header and library paths
link_directories(${LIBRARY_OUTPUT_PATH})
set(CMAKE_TEMP_LIBRARY_PATH "${PROJECT_BINARY_DIR}/lib")

add_subdirectory(lib)
add_subdirectory(tools)
add_subdirectory(testing)



