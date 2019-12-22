# Set variable, and make it visible to parent CMakeList for inclusion
set(GUI_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/console.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/console.cpp
    PARENT_SCOPE
)
