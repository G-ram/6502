#include <sstream>
#include <fstream>

#include "utloader.h"
#include "headers/log.h"

UnitTestLoader::UnitTestLoader(std::string filename) {
    std::ifstream input(filename, std::ios::binary);
    std::stringstream buffer;
    buffer << input.rdbuf();
    raw = buffer.str();

    //Copy contents into memory
    reg.PC.udw = 0x0600; // Entry Point
    reg.S.udw = 0x20; // Stack Pointer
    mem.copyTo(raw, reg.PC);
}
