#ifndef CPU_H
#define CPU_C_H

#include "types.h"
#include "mem.h"
#include "reg.h"
#include "decoder.h"

class CPU {
public:
    Mem *mem;
private:
    Reg *reg;
    Decoder *decoder;
public:
    CPU(word, word);
    void step();
    size_t getCycles();
};

#endif
