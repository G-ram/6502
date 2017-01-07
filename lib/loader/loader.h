#ifndef LOADER_H
#define LOADER_H

#include "../reg.h"
#include "../mem.h"

class Loader {
protected:
    std::string raw;
    Mem mem;
    Reg reg;
public:
    Loader() : mem(&reg) {};
    Mem *getMem() { return &mem; };
    Reg *getReg() { return &reg; };
};

#endif
