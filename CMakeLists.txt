cmake_minimum_required(VERSION 3.13)
project(simple_audio_c99 C)

set(CMAKE_C_STANDARD 99)

add_executable(simple_audio_c99 main.c load.h err.h err.c load.c play.h play.c)

target_link_libraries(simple_audio_c99 -lavutil -lavcodec -lavformat -lavresample -lao)