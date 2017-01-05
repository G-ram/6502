#include <sstream>
#include <fstream>

#include "utloader.h"

UnitTestLoader::UnitTestLoader(std::string filename) {
    std::ifstream input(filename, std::ios::binary);
    std::stringstream buffer;
    buffer << input.rdbuf();
    raw = buffer.str();

    word startAddr;
    //Copy contents into memory
    mem.copyTo(raw, startAddr);
    // Set PC
    memcpy(&reg.PC.udw, &startAddr, sizeof(word));
}
