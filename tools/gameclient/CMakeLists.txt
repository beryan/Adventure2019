
add_executable(gameclient
        gameclient.cpp
        GameWindow.cpp
)

set_target_properties(gameclient
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
                      PREFIX ""
)

find_package(Curses REQUIRED)
find_package(Threads REQUIRED)

target_include_directories(gameclient
  PRIVATE
  ${CURSES_INCLUDE_DIR}
)

target_link_libraries(gameclient
  networking
  ${CURSES_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
)

install(TARGETS gameclient
  RUNTIME DESTINATION bin
)

