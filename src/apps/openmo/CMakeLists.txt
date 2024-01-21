set(OPENMO_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src/apps/openmo)

set(OPENMO_HEADERS)

set(OPENMO_SOURCES
	main.cpp)

add_executable(openmo ${OPENMO_SOURCES} ${OPENMO_HEADERS})
target_precompile_headers(openmo PRIVATE ${CMAKE_SOURCE_DIR}/src/pch.hpp)
target_link_libraries(openmo PRIVATE common graphics glad SDL2main SDL2-static)
