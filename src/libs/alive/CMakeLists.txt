set(ALIVE_HEADERS
    scene.hpp
    xinfile.hpp
    xrgfile.hpp)

set(ALIVE_SOURCES
    scene.cpp
    xinfile.cpp
    xrgfile.cpp)

add_library(alive STATIC ${ALIVE_HEADERS} ${ALIVE_SOURCES})
set_target_properties(alive PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}$<$<CONFIG:Debug>:/debug>/lib)
set_target_properties(alive PROPERTIES DEBUG_POSTFIX "d")
target_precompile_headers(alive PRIVATE ${CMAKE_SOURCE_DIR}/src/pch.hpp)
target_link_libraries(alive PRIVATE common pugixml)

