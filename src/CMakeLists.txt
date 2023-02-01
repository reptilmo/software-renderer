cmake_minimum_required(VERSION 3.23)

add_executable(renderer
    display.h
    display.c
    main.c
)

target_compile_options(renderer PRIVATE -Wall)

target_include_directories(renderer PUBLIC
    ${CMAKE_SOURCE_DIR}/3rd-party/SDL/include
)

target_link_libraries(renderer PUBLIC
    SDL2-static
    SDL2main
)
