
add_executable(gameserver
        gameserver.cpp
)

set_target_properties(gameserver
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
                      PREFIX ""
)

find_package(Threads REQUIRED)

target_link_libraries(gameserver
  networking
  model
  ${CMAKE_THREAD_LIBS_INIT}
)

install(TARGETS gameserver
  RUNTIME DESTINATION bin
)
