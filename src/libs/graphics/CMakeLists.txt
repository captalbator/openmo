set(GRAPHICS_HEADERS
	camera.hpp
	renderer.hpp
	shader.hpp
	mesh.hpp
	window.hpp)

set(GRAPHICS_SOURCES
	camera.cpp
	renderer.cpp
	shader.cpp
	mesh.cpp
	window.cpp)

add_library(graphics STATIC ${GRAPHICS_HEADERS} ${GRAPHICS_SOURCES})
set_target_properties(graphics PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}$<$<CONFIG:Debug>:/debug>/lib)
set_target_properties(graphics PROPERTIES DEBUG_POSTFIX "d")
target_precompile_headers(graphics PRIVATE ${CMAKE_SOURCE_DIR}/src/pch.hpp)
target_link_libraries(graphics PRIVATE common glad ${OPENGL_LIBRARIES})

if(MSVC)
    target_link_libraries(graphics PRIVATE SDL2::SDL2)
else()
    target_link_libraries(graphics PRIVATE ${SDL2_LIBRARIES})
endif()
