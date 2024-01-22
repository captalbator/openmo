set(NIF_HEADERS
    base.hpp
    blockptr.hpp
    controller.hpp
    effects.hpp
    extradata.hpp
    geom.hpp
    keys.hpp
    nifreader.hpp
    nifstream.hpp
    nodes.hpp
    objects.hpp
    particles.hpp
    shaders.hpp
    skin.hpp
    types.hpp)

set(NIF_SOURCES
    controller.cpp
    effects.cpp
    extradata.cpp
    geom.cpp
    nifreader.cpp
    nifstream.cpp
    nodes.cpp
    objects.cpp
    particles.cpp
    shaders.cpp
    skin.cpp)

add_library(nif STATIC ${NIF_HEADERS} ${NIF_SOURCES})
set_target_properties(nif PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
target_precompile_headers(nif PRIVATE ${CMAKE_SOURCE_DIR}/src/pch.hpp)
target_link_libraries(nif PRIVATE common)

#target_sources(gabbit PRIVATE ${NIF_HEADERS} ${NIF_SOURCES})
