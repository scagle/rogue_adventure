# Set variable, and make it visible to parent CMakeList for inclusion
set(DATATYPE_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/array2d.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/position.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tile.hpp
    PARENT_SCOPE
)
