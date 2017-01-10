#ifndef CPU_H
#define CPU_H

#include "types.h"
#include "mem.h"
#include "reg.h"
#include "decoder.h"

class CPU {
public:
    Mem *mem;
private:
    unsigned long long cycles;
    Reg *reg;
    Decoder *decoder;
public:
    CPU(word, word);
    void step();
    unsigned long long getCycles() { return cycles; };
};

#endif
