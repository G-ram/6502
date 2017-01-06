#ifndef MEM_H
#define MEM_H

#include <string>
#include <vector>

#include "types.h"
#include "reg.h"

#define MEM_SIZE 0x10000

class Peripheral;

enum Mode{IMM, ABS, ZERO, IND_ABS, ABSX,
        ABSY, ZEROX, ZEROY, IDX_IND,
        IND_IDX, REL, ACC, VOID};

extern std::string modeName[13];

class Mem {
private:
    Reg *reg;
    unsigned char data[MEM_SIZE];
    std::vector<Peripheral *> peripherals;
public:
    Mem(Reg *reg) : reg(reg) {
        memset(data, 0, MEM_SIZE * sizeof(unsigned char));
    };
    word translate(word, Mode);
    word load(word, Mode);
    void store(word, word, Mode);
    void push(word);
    word pop();
    void copyTo(std::string, word);
    std::string dump();
    void connect(Peripheral *p) { peripherals.push_back(p); }
    void broadcast();
};

#endif
