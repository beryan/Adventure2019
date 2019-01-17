
add_executable(chatserver
  chatserver.cpp
)

set_target_properties(chatserver
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
                      PREFIX ""
)

find_package(Threads REQUIRED)

target_link_libraries(chatserver
  networking
  ${CMAKE_THREAD_LIBS_INIT}
)

target_include_directories(chatserver PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

install(TARGETS chatserver
  RUNTIME DESTINATION bin
)

