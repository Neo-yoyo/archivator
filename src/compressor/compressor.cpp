#include "compressor.hpp"


/// инициализируем конструктор класса compressor
compressor::compressor(compressor_config& config_) {
    _config = config_;
}


/// реализация метода compress класса compressor
void compressor::compress() {

    switch (_config.compression) {

    /// для доступа к перечислению compression_type используем оператор расширения области видимости "::"
    case compression_type::zstd:
        compressor_zstd();
        break;

    case compression_type::zlib:
        compressor_zlib();
        break;
    }

}


void compressor::compressor_zstd() {

    /// _config.path_in - доступ к папке с входными данными через экземпляр структуры _config
    for (const auto & entry : std::filesystem::directory_iterator(_config.path_in)) {
        std::cout << entry.path() << std::endl;

        std::string in_filename = entry.path().string();

        /// open the file:
        std::streampos in_file_size = entry.file_size();
        std::ifstream in_file(in_filename, std::ios::binary);

        /// read the data:
        std::vector<unsigned char> in_buff(in_file_size);
        in_file.read((char*) &in_buff[0], in_file_size);

        in_file.close();

        /// вычисляем мах возможный размер выходного буфера,
        /// чтобы не выйти за область памяти
        size_t const out_buff_size = ZSTD_compressBound(in_file_size);
        /// выделяем место в выходном буфере
        std::vector<unsigned char> out_buff(out_buff_size);

        /// Сжатие
        /// If you are doing many compressions, you may want to reuse the context.
        /// See the multiple_simple_compression.c example.
        size_t const out_file_size = ZSTD_compress((void*) &out_buff[0], out_buff_size, (void*) &in_buff[0], in_file_size, _config.compression_level);
        if (ZSTD_isError(out_file_size)) {
            std::cout << ZSTD_getErrorName(out_file_size) << std::endl;
            continue;
        }

        /// формируем имя выходного файла
        std::string out_filename = _config.path_out + "/" + entry.path().filename().string() + ".arch";

        /// открываем выходной файл
        std::ofstream out_file(out_filename);

        /// записываем выходной файл
        out_file.write((char*) &out_buff[0], out_file_size);

        out_file.close();

    }
}


void compressor::compressor_zlib() {

    for (const auto & entry : std::filesystem::directory_iterator(_config.path_in)) {
        std::cout << entry.path() << std::endl;

        std::string in_filename = entry.path().string();
        std::string out_filename = _config.path_out + "/" + entry.path().filename().string() + ".arch";

        FILE* in_file = fopen(in_filename.c_str(), "rb");
        FILE* out_file = fopen(out_filename.c_str(), "wb");

        const int CHUNK = 16384;
        const int level = _config.compression_level;

        int ret, flush;
        unsigned have;
        z_stream strm;
        unsigned char in_buff[CHUNK];
        unsigned char out[CHUNK];

        /* allocate deflate state */
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        ret = deflateInit(&strm, level);
        if (ret != Z_OK)
            return;

        /* compress until end of file */
        do {
            strm.avail_in = fread(in_buff, 1, CHUNK, in_file);
            if (ferror(in_file)) {
                deflateEnd(&strm);
                return;
            }
            flush = feof(in_file) ? Z_FINISH : Z_NO_FLUSH;
            strm.next_in = in_buff;

            /* run deflate() on input until output buffer not full, finish
               compression if all of source has been read in */
            do {
                strm.avail_out = CHUNK;
                strm.next_out = out;
                ret = deflate(&strm, flush);    /* no bad return value */
                //assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
                have = CHUNK - strm.avail_out;
                if (fwrite(out, 1, have, out_file) != have || ferror(out_file)) {
                    deflateEnd(&strm);
                    return;
                }
            } while (strm.avail_out == 0);
            //assert(strm.avail_in == 0);     /* all input will be used */

            /* done when last data in file processed */
        } while (flush != Z_FINISH);
        //assert(ret == Z_STREAM_END);        /* stream will be complete */

        /* clean up and return */
        deflateEnd(&strm);


        fclose(in_file);
        fclose(out_file);
    }
}

