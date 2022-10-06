#include "decompressor.hpp"

/// инициализируем конструктор класса
decompressor::decompressor(decompressor_config& config_) {
    _config = config_;
}


void decompressor::decompress() {

    switch (_config.decompression) {
        case decompression_type::zstd:
            decompressor_zstd();
        break;

        case decompression_type::zlib:
            decompressor_zlib();
        break;
    }

}



void decompressor::decompressor_zstd () {

    for (const auto & entry : std::filesystem::directory_iterator(_config.path_in)) {
        //std::cout << entry.path() << std::endl;

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
        size_t const out_buff_size = ZSTD_getFrameContentSize((void*) &in_buff[0], in_file_size);
        /// выделяем место в выходном буфере
        std::vector<unsigned char> out_buff(out_buff_size);

        /// Сжатие
        /// If you are doing many compressions, you may want to reuse the context.
        /// See the multiple_simple_compression.c example.
        size_t const out_file_size = ZSTD_decompress((void*) &out_buff[0], out_buff_size, (void*) &in_buff[0], in_file_size);
        if (ZSTD_isError(out_file_size)) {
            std::cout << ZSTD_getErrorName(out_file_size) << std::endl;
            continue;
        }

        /// формируем имя выходного файла
        std::string out_filename = _config.path_out + "/" + entry.path().filename().string() + ".decompress";

        /// открываем выходной файл
        std::ofstream out_file(out_filename);

        /// записываем выходной файл
        out_file.write((char*) &out_buff[0], out_file_size);

        out_file.close();
    }

}


void decompressor::decompressor_zlib () {

    for (const auto & entry : std::filesystem::directory_iterator(_config.path_in)) {
        std::cout << entry.path() << std::endl;

        std::string in_filename = entry.path().string();
        std::string out_filename = _config.path_out + "/" + entry.path().filename().string() + ".arch";

        FILE* in_file = fopen(in_filename.c_str(), "rb");
        FILE* out_file = fopen(out_filename.c_str(), "wb");

        const int CHUNK = 16384;

        int ret, flush;
        unsigned have;
        z_stream strm;
        unsigned char in_buff[CHUNK];
        unsigned char out[CHUNK];

        /* allocate inflate state */
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        strm.avail_in = 0;
        strm.next_in = Z_NULL;
        ret = inflateInit(&strm);
        if (ret != Z_OK)
            return;

        /* decompress until deflate stream ends or end of file */
        do {
            strm.avail_in = fread(in_buff, 1, CHUNK, in_file);
            if (ferror(in_file)) {
                inflateEnd(&strm);
                return;
            }
            if (strm.avail_in == 0)
                break;
            strm.next_in = in_buff;

        /* run inflate() on input until output buffer not full */
            do {
                strm.avail_out = CHUNK;
                strm.next_out = out;
                ret = inflate(&strm, Z_NO_FLUSH);
                //assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
                switch (ret) {
                case Z_NEED_DICT:
                    ret = Z_DATA_ERROR;     /* and fall through */
                case Z_DATA_ERROR:
                case Z_MEM_ERROR:
                    inflateEnd(&strm);
                    return;
                }
                have = CHUNK - strm.avail_out;
                if (fwrite(out, 1, have, out_file) != have || ferror(out_file)) {
                    inflateEnd(&strm);
                    return;
                }

            } while (strm.avail_out == 0);

            /* done when inflate() says it's done */
        } while (ret != Z_STREAM_END);

        /* clean up and return */
        inflateEnd(&strm);


        fclose(in_file);
        fclose(out_file);
    }
}
