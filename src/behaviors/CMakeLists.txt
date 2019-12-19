# Set variable, and make it visible to parent CMakeList for inclusion
set(BEHAVIORS_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/ai.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ai.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/attacker.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/attacker.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/destructible.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/destructible.hpp
    PARENT_SCOPE
)
