set(GABBIT_HEADERS )

set(GABBIT_SOURCES
    main.cpp)

add_executable(gabbit ${GABBIT_SOURCES} ${GABBIT_HEADERS})
set_target_properties(gabbit PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}$<$<CONFIG:Debug>:/debug>/bin)
target_precompile_headers(gabbit PRIVATE ${CMAKE_SOURCE_DIR}/src/pch.hpp)
target_link_libraries(gabbit PRIVATE common graphics glad SDL2main SDL2-static)
