cmake_minimum_required(VERSION 3.12)

add_executable(compressor "main.cpp")

target_link_libraries(compressor PRIVATE
    zstd)
