cmake_minimum_required(VERSION 3.23)

add_executable(renderer
    system.h
    darray.h
    darray.c
    config.h
    config.c
    input.h
    input.c
    vec.h
    mat.h
    color.h
    mesh.h
    mesh.c
    texture.h
    texture.c
    display.h
    display.c
    triangle.h
    triangle_shaded.c
    triangle_textured.c
    clipping.h
    clipping.c
    frustum.h
    frustum.c
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
