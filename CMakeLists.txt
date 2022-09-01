cmake_minimum_required(VERSION 3.12)

project(archivator)

#добавляем функцию FC (чтобы исходники другого проекта были доступны в моем проекте)
include(FetchContent)

# добавляем пакет
find_package(zstd QUIET)

#if(NOT zstd_FOUND)

#    #подключаем репозиторий
#    FetchContent_Declare(
#        zstd
#        GIT_REPOSITORY git@github.com:facebook/zstd.git
#    )
#    FetchContent_MakeAvailable(zstd)
#endif()

add_subdirectory(src/compressor)
add_subdirectory(src/decompressor)


