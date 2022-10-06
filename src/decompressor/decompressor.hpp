#ifndef DECOMPRESSOR_HPP
#define DECOMPRESSOR_HPP

#include <zstd.h>
#include <zlib.h>

#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <filesystem>


/// создаем КЛАСС enum для методов zstd и zlib
enum class decompression_type {
    zstd,
    zlib
};


/// создаем структуру, которая будет являться полем класса
struct decompressor_config {
    std::string path_in;
    std::string path_out;
    ///decompression - экземпляр класса enum (это нужно, чтобы использовать оператор swich)
    decompression_type decompression;
};


class decompressor
{
public:
    /// создаем конструктор класса
    decompressor(decompressor_config& config_);
    void decompress();

private:
    void decompressor_zstd();

    void decompressor_zlib();

    /// объявляем экземпляр структуры
    decompressor_config _config;

};

#endif // DECOMPRESSOR_HPP
