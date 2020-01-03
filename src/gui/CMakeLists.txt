# Set variable, and make it visible to parent CMakeList for inclusion
set(GUI_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/console.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/console.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gui.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/menu.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/menu.cpp
    PARENT_SCOPE
)
