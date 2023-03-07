cmake_minimum_required(VERSION 3.23)

project("software_renderer")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED On)
set(CMAKE_C_EXTENSIONS Off)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_subdirectory(3rd-party/SDL)
add_subdirectory(src)
