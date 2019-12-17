# Libtcod
find_package(Libtcod REQUIRED)

# Threads
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

set(BASE_SOURCES 
    actor.cpp
    actor.hpp
    resource_handler.cpp
    resource_handler.hpp
    engine.cpp
    engine.hpp
    main.cpp
    map.cpp
    map.hpp
)

add_subdirectory(datatypes)
add_subdirectory(enums)

add_executable(cursed ${BASE_SOURCES} ${DATATYPES_SOURCES} ${ENUMS_SOURCES} ${INCLUDE_SOURCES})

target_include_directories(cursed PUBLIC ${LIBTCOD_INCLUDE_DIRS})

target_link_libraries(cursed Threads::Threads)
target_link_libraries(cursed ${LIBTCOD_LIBRARIES})


message("Copying binary to <project>/build")
add_custom_command(TARGET cursed
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:cursed> "${CMAKE_BINARY_DIR}/"
)

message("Copying font to <project>/build/terminal.png")
configure_file("terminal.png" "${CMAKE_BINARY_DIR}" COPYONLY)
