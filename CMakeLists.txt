cmake_minimum_required(VERSION 3.12)

project(archivator)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

#добавляем функцию FC (чтобы исходники другого проекта были доступны в моем проекте)
include(FetchContent)

find_package(Zstd REQUIRED)
find_package(ZLIB REQUIRED)

FetchContent_Declare(
    tomlplusplus
    GIT_REPOSITORY git@github.com:marzer/tomlplusplus.git
)
FetchContent_MakeAvailable(tomlplusplus)

add_subdirectory(src/compressor)
add_subdirectory(src/decompressor)


