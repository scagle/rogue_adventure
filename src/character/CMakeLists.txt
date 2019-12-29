
add_subdirectory(behaviors)

# Set variable, and make it visible to parent CMakeList for inclusion
set(CHARACTER_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/actor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/actor.hpp
    ${BEHAVIORS_SOURCES}
    PARENT_SCOPE
)

