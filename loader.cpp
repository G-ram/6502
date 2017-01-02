#include <sstream>
#include <fstream>

#include "loader.h"

Loader::Loader(std::string filename) : mem(MEM_SIZE, reg), reg() {
    std::ifstream input(filename, std::ios::binary);
    std::stringstream buffer;
    buffer << input.rdbuf();
    raw = buffer.str();

    unsigned short startAddr;
    memcpy(&startAddr, raw.c_str(), sizeof(unsigned short));
    //Copy contents into memory
    raw.erase(0, 2); // Get rid of the file header
    word initAddr;
    memcpy(&initAddr, raw.c_str(), sizeof(word));
    mem.copyTo(raw, initAddr);

    // Set PC
    startAddr += 12; // May Change depending on the file format
    memcpy(&reg.PC.udw, &initAddr, sizeof(word));;
}
