#ifndef COMPRESSOR_HPP
#define COMPRESSOR_HPP

#include <zstd.h>
#include <zlib.h>

#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <filesystem>


/// создаем КЛАСС enum
enum class compression_type {
    zstd,
    zlib
};

/// вместо перечесления полей в классе создаем отдельную структуру со всеми параметрами (полями)
struct compressor_config {
    std::string path_in;
    std::string path_out;
    int compression_level;
    compression_type compression;
};


class compressor
{
public:
    /// конструктор класса
    compressor(compressor_config& config_);

    void compress();

private:
    void compressor_zstd();

    void compressor_zlib();

    /// структура является полем класса
    compressor_config _config;
};

#endif // COMPRESSOR_HPP
