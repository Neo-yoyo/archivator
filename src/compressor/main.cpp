
#include "compressor.hpp"

#include <toml++/toml.h>

#include <chrono>

using namespace std::chrono;
using default_duration = std::chrono::milliseconds;
using my_clock = std::chrono::high_resolution_clock;

int main(int argc, const char** argv) {

    /// объявляем экземпляр структуры
    compressor_config _compressor_config;

    /// доступ к файлу config.toml
    auto config = toml::parse_file("config.toml");

    /// инициализируем поля структуры в файле config.toml
    _compressor_config.path_in = config["compressor"]["path_in"].value_or("");
    _compressor_config.path_out = config["compressor"]["path_out"].value_or("");
    _compressor_config.compression_level = config["compressor"]["compression_level"].value_or(0);

    std::string compression_str = config["compressor"]["compression"].value_or("");

    if (compression_str == "zlib")
        /// если переменная compression экземпляра структуры _compressor_config совпадает с элементом класса enam "zlib", то сжимаем с помощью zlib
        _compressor_config.compression = compression_type::zlib;
    else if (compression_str == "zstd")
        _compressor_config.compression = compression_type::zstd;

    /// объявляем экземпляр класса _compressor класса compressor
    compressor _compressor(_compressor_config);

    auto start_ts = std::chrono::duration_cast<default_duration>(my_clock::now().time_since_epoch());

    /// вызываем метод compress экземпляра класса _compressor класса compressor
    _compressor.compress();

    auto end_ts = std::chrono::duration_cast<default_duration>(my_clock::now().time_since_epoch());
    std::cout << (end_ts - start_ts).count() << std::endl;

    return 0;
}
