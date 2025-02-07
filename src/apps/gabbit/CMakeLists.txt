set(GABBIT_HEADERS 
    editor.hpp)

set(GABBIT_SOURCES
    editor.cpp
    main.cpp)

add_executable(gabbit ${GABBIT_SOURCES} ${GABBIT_HEADERS})
set_target_properties(gabbit PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}$<$<CONFIG:Debug>:/debug>/bin)
target_precompile_headers(gabbit PRIVATE ${CMAKE_SOURCE_DIR}/src/pch.hpp)
target_link_libraries(gabbit PRIVATE common alive nif graphics glad SDL2::SDL2main SDL2::SDL2 imgui)
