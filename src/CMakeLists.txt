cmake_minimum_required(VERSION 3.23)

add_executable(renderer
    main.c
)

target_compile_options(renderer PRIVATE -Wall)
