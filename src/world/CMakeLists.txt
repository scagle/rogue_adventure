# Set variable, and make it visible to parent CMakeList for inclusion
set(WORLD_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/world.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/world.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/map.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/map.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/procedural_generator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/procedural_generator.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/zone.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/zone.hpp
    PARENT_SCOPE
)
