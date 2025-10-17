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
)

target_include_directories(${T} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(${T} PRIVATE -Wall -Wextra -Wpedantic)
