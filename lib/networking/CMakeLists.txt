
add_library(networking
  src/Server.cpp
  src/Client.cpp
)

target_include_directories(networking
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${Boost_INCLUDE_DIR}
)

target_link_libraries(networking
  PRIVATE
    ${Boost_LIBRARIES}
)

set_target_properties(networking
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
)

install(TARGETS networking
  ARCHIVE DESTINATION lib
)

set_target_properties(networking
        PROPERTIES
        COMPILE_FLAGS "${CXX_COVERAGE_FLAGS}"
        )