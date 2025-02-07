cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME OpenMO)
project(${PROJECT_NAME} LANGUAGES C CXX)

aux_source_directory(./src SRC_LIST)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(PROJECT_BIN_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BIN_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BIN_DIRECTORY}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BIN_DIRECTORY}/lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BIN_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BIN_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BIN_DIRECTORY}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BIN_DIRECTORY}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BIN_DIRECTORY}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BIN_DIRECTORY}/lib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_OPENMO "Build OpenMO" OFF)
option(BUILD_GABBIT "Build Gabbit" ON)

add_subdirectory(extern)

find_package(OpenGL REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/extern
                    ${CMAKE_SOURCE_DIR}/extern/nfd/src/include
                    ${CMAKE_SOURCE_DIR}/extern/glad/include
                    ${CMAKE_SOURCE_DIR}/extern/pugixml/src
                    ${CMAKE_SOURCE_DIR}/extern/fmt/include
                    ${CMAKE_SOURCE_DIR}/extern/SDL2/include
                    ${CMAKE_SOURCE_DIR}/extern/glm
                    ${CMAKE_SOURCE_DIR}/src)

add_subdirectory(src/libs/alive)
add_subdirectory(src/libs/common)
add_subdirectory(src/libs/graphics)
add_subdirectory(src/libs/nif)

if(BUILD_OPENMO)
    add_subdirectory(src/apps/openmo)
endif()

if(BUILD_GABBIT)
    add_subdirectory(src/apps/gabbit)
endif()
