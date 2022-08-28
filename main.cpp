#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <filesystem>

typedef unsigned char BYTE;

int main()
{

    std::string path_in = "/home/neo_yoyo/Документы/trash/in";
    std::string path_out = "/home/neo_yoyo/Документы/trash/out";


    for (const auto & entry : std::filesystem::directory_iterator(path_in)) {
        std::cout << entry.path() << std::endl;

        std::string filename = entry.path().string();

        // open the file:
        std::streampos fileSize = entry.file_size();
        std::ifstream file(filename, std::ios::binary);

        // read the data:
        std::vector<BYTE> fileData(fileSize);
        file.read((char*) &fileData[0], fileSize);


        std::string filename_out = path_out + "/" + entry.path().filename().string() + ".arch";

        std::ofstream file_out(filename_out);

        file_out.write((char*) &fileData[0], fileSize);
    }



    return 0;
}
