# Set variable, and make it visible to parent CMakeList for inclusion
set(BEHAVIORS_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/ai.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ai.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/attacker.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/attacker.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/destructible.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/destructible.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pickable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pickable.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/inventory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/inventory.hpp
    PARENT_SCOPE
)
