#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <filesystem>

#include <zstd.h>

typedef unsigned char BYTE;

void decompressor_zstd (const std::string& path_in, const std::string& path_out) {

    for (const auto & entry : std::filesystem::directory_iterator(path_in)) {
        std::cout << entry.path() << std::endl;

        std::string in_filename = entry.path().string();

        /// open the file:
        std::streampos in_file_size = entry.file_size();
        std::ifstream in_file(in_filename, std::ios::binary);

        /// read the data:
        std::vector<BYTE> in_buff(in_file_size);
        in_file.read((char*) &in_buff[0], in_file_size);

        in_file.close();

        /// вычисляем мах возможный размер выходного буфера,
        /// чтобы не выйти за область памяти
        size_t const out_buff_size = ZSTD_getFrameContentSize((void*) &in_buff[0], in_file_size);
        /// выделяем место в выходном буфере
        std::vector<BYTE> out_buff(out_buff_size);

        /// Сжатие
        /// If you are doing many compressions, you may want to reuse the context.
        /// See the multiple_simple_compression.c example.
        size_t const out_file_size = ZSTD_decompress((void*) &out_buff[0], out_buff_size, (void*) &in_buff[0], in_file_size);
        if (ZSTD_isError(out_file_size)) {
            std::cout << ZSTD_getErrorName(out_file_size) << std::endl;
            continue;
        }

        /// формируем имя выходного файла
        std::string out_filename = path_out + "/" + entry.path().filename().string() + ".decompress";

        /// открываем выходной файл
        std::ofstream out_file(out_filename);

        /// записываем выходной файл
        out_file.write((char*) &out_buff[0], out_file_size);

        out_file.close();
    }

}


int main(int argc, const char** argv) {

    std::string path_in = "/home/neo_yoyo/Документы/trash/compressed";
    std::string path_out = "/home/neo_yoyo/Документы/trash/decompressed";

    decompressor_zstd (path_in, path_out);


    return 0;
}
