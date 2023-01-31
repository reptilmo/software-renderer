cmake_minimum_required(VERSION 3.23)

add_executable(renderer
    main.c
)

target_compile_options(renderer PRIVATE -Wall)

target_include_directories(renderer SYSTEM
  PRIVATE ${CMAKE_SOURCE_DIR}/3rd-party/SDL/include
)

target_link_libraries(renderer PUBLIC
    SDL3-static
)
