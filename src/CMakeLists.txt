# Threads
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

set(BASE_SOURCES 
    main.cpp
)

add_executable(curses ${BASE_SOURCES})

target_link_libraries(curses Threads::Threads)

message("Copying binary to <project>/build")
add_custom_command(TARGET curses
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:curses> "${CMAKE_BINARY_DIR}/"
)
