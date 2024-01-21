set(COMMON_HEADERS
	log.hpp)

set(COMMON_SOURCES
	log.cpp)

add_library(common STATIC ${COMMON_HEADERS} ${COMMON_SOURCES})
set_target_properties(common PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}$<$<CONFIG:Debug>:/debug>/lib)
set_target_properties(common PROPERTIES DEBUG_POSTFIX "d")
target_precompile_headers(common PRIVATE ${CMAKE_SOURCE_DIR}/src/pch.hpp)
target_link_libraries(common PRIVATE fmt)
