set(GLAD_SOURCES
    src/gl.c)

set(GLAD_HEADERS
    include/glad/gl.h
    include/KHR/khrplatform.h)

add_library(glad ${GLAD_SOURCES} ${GLAD_HEADERS})
target_include_directories(glad PUBLIC include)