cmake_minimum_required(VERSION 3.23)

add_executable(renderer
    system.h
    darray.h
    darray.c
    vec.h
    vec.c
    mat.h
    mesh.h
    mesh.c
    display.h
    display.c
    sort.h
    sort.c
    renderer.h
    renderer.c
    main.c
)

target_compile_options(renderer PRIVATE /W4)

target_include_directories(renderer PUBLIC
    ${CMAKE_SOURCE_DIR}/3rd-party/SDL/include
)

target_link_libraries(renderer PUBLIC
    SDL2-static
    SDL2main
)
