cmake_minimum_required(VERSION 3.5)
project(melyze)

set(CMAKE_CXX_STANDARD 17)

if (APPLE OR WIN32)
    message(FATAL_ERROR "This project is compatible with either Linux or Android operating systems")
endif (APPLE OR WIN32)

file(GLOB_RECURSE SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

set(INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

find_library(lib-log log)

add_executable(melyze ${SOURCE})
target_include_directories(melyze PRIVATE ${INCLUDES})

add_subdirectory(sample)
