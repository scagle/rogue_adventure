# Set variable, and make it visible to parent CMakeList for inclusion
set(ENUMS_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/tile_type.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/game_status.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/entry_point.hpp
    PARENT_SCOPE
)
