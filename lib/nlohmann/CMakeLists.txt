add_library(json INTERFACE)

target_sources(json INTERFACE "${PROJECT_SOURCE_DIR}/lib/nlohmann/include/json.hpp")

target_include_directories(json INTERFACE
        "${PROJECT_SOURCE_DIR}/lib/nlohmann/include"
        )
