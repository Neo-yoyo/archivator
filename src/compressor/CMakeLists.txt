cmake_minimum_required(VERSION 3.12)
include(FetchContent)

FetchContent_Declare(
    tomlplusplus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG        v3.2.0
)
FetchContent_MakeAvailable(tomlplusplus)

add_executable(compressor
    "main.cpp"
    "compressor.cpp"
    )

find_package(ZLIB REQUIRED)
find_path(ZSTD_INCLUDE_DIR zstd.h)
#find_package(ZSTD REQUIRED)
include_directories(${ZSTD_INCLUDE_DIR})
#find_package(tomlplusplus REQUIRED)

target_link_libraries(compressor PRIVATE
    zstd
    ZLIB::ZLIB
    tomlplusplus::tomlplusplus)


