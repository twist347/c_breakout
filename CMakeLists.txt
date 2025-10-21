cmake_minimum_required(VERSION 3.20)

set(T c_breakout)

project(${T})

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(RAYLIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/raylib-5.5_linux_amd64)

add_executable(
        ${T}
        main.c
        src/event_system.c
        src/audio.c
        src/util.c
        src/ball.c
        src/brick.c
        src/game_screen.c
        src/game_state.c
        src/hud.c
        src/menu.c
        src/texture.c
        src/paddle.c
        src/game.c
)

target_include_directories(${T} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${T} PRIVATE ${RAYLIB_DIR}/include)
target_link_directories(${T} PRIVATE ${RAYLIB_DIR}/lib)
target_link_libraries(${T} PRIVATE raylib m)
target_compile_options(${T} PRIVATE -Wall -Wextra -Wpedantic)

target_compile_definitions(${T} PRIVATE ASSETS_DIR="/assets")

add_custom_command(TARGET ${T} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/${ASSETS_DIR}
        $<TARGET_FILE_DIR:${T}>/${ASSETS_DIR})
