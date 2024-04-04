set(IMGUI_HEADERS
    imconfig.h
    imgui.h
    imgui_impl_opengl3.h
    imgui_impl_opengl3_loader.h
    imgui_impl_sdl2.h
    imgui_internal.h
    imstb_rectpack.h
    imstb_textedit.h
    imstb_truetype.h)

set(IMGUI_SOURCES
    imgui.cpp
    imgui_draw.cpp
    imgui_impl_opengl3.cpp
    imgui_impl_sdl2.cpp
    imgui_tables.cpp
    imgui_widgets.cpp)

add_library(imgui STATIC ${IMGUI_HEADERS} ${IMGUI_SOURCES})
set_target_properties(imgui PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
target_include_directories(imgui PRIVATE ${CMAKE_SOURCE_DIR}/extern/SDL2/include)