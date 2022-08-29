cmake_minimum_required(VERSION 3.12)

add_executable(decompressor "main.cpp")

target_link_libraries(decompressor PRIVATE
    zstd)
