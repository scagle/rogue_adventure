# Fmt PrintF like formatter 
find_package(fmt REQUIRED)

# Libtcod
find_package(Libtcod REQUIRED)

# Threads
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

set(BASE_SOURCES 
    container_component.cpp
    container_component.hpp
    resource_handler.cpp
    resource_handler.hpp
    engine.cpp
    engine.hpp
    camera.cpp
    camera.hpp
    options.hpp
    main.cpp
)

add_subdirectory(world) # WorldTiles, Maps
add_subdirectory(character) # Actors
add_subdirectory(datatypes)
add_subdirectory(enums)
add_subdirectory(gui) # Consoles

add_executable(rogue_adventure ${BASE_SOURCES} ${WORLD_SOURCES} ${CHARACTER_SOURCES} ${BEHAVIOR_SOURCES} ${DATATYPES_SOURCES} ${ENUMS_SOURCES} ${GUI_SOURCES} ${INCLUDE_SOURCES})

target_include_directories(rogue_adventure PUBLIC ${LIBTCOD_INCLUDE_DIRS})

target_link_libraries(rogue_adventure fmt::fmt)
target_link_libraries(rogue_adventure ${LIBTCOD_LIBRARIES})
target_link_libraries(rogue_adventure Threads::Threads)


message("Copying binary to <project>/build")
add_custom_command(TARGET rogue_adventure
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:rogue_adventure> "${CMAKE_BINARY_DIR}/"
)

message("Copying font to <project>/build/terminal.png")
configure_file("terminal.png" "${CMAKE_BINARY_DIR}" COPYONLY)
