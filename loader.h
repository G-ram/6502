#ifndef LOADER_H
#define LOADER_H

#include <string>

#define MEM_SIZE 0x10000

class Loader() {
    std::string raw;
    Mem mem;
    Reg reg;
public:
    Loader(std::string);
    Mem &getMem() { return mem; };
    Reg &getReg() { return reg; };
}
#endif
