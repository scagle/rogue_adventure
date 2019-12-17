# Set variable, and make it visible to parent CMakeList for inclusion
set(ENUMS_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/tile_type.hpp
    PARENT_SCOPE
)
