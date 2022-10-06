
#include "decompressor.hpp"

#include <toml++/toml.h>

//typedef unsigned char BYTE;


int main(int argc, const char** argv) {

    /// создаем экземпляр структуры
    decompressor_config _decompressor_config;


    auto config = toml::parse_file("config.toml");

    _decompressor_config.path_in = config["decompressor"]["path_in"].value_or("");
    _decompressor_config.path_out = config["decompressor"]["path_out"].value_or("");

    std::string decompression_str = config["decompressor"]["decompression"].value_or("");

    if (decompression_str == "zlib")
        _decompressor_config.decompression = decompression_type::zlib;
    else if (decompression_str == "zstd")
        _decompressor_config.decompression = decompression_type::zstd;


    /// создаем экземпляр класса, полем которого является структура
    decompressor _decompressor(_decompressor_config);


    /// вызываем метод decompress экземпляра класса _decompressor класса decompressor
    _decompressor.decompress();

    return 0;
}
