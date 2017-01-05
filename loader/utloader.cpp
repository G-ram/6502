#include <sstream>
#include <fstream>

#include "utloader.h"
#include "../log.h"

UnitTestLoader::UnitTestLoader(std::string filename) {
    std::ifstream input(filename, std::ios::binary);
    std::stringstream buffer;
    buffer << input.rdbuf();
    raw = buffer.str();

    //Copy contents into memory, automatically sets PC
    reg.PC.udw = 0x0600; // Start address
    //reg.S.udw = 0x200; // Stack Pointer Initialization
    mem.copyTo(raw, reg.PC);
}
