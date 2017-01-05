#ifndef MEM_H
#define MEM_H

#include <string>
#include <vector>

#include "types.h"
#include "reg.h"
#include "peripheral/peripheral.h"

#define MEM_SIZE 0x10000

enum Mode{IMM, ABS, ZERO, IND_ABS, ABSX,
        ABSY, ZEROX, ZEROY, IDX_IND,
        IND_IDX, REL, ACC, VOID};

extern std::string modeName[13];

class Mem {
private:
    Reg *reg;
    std::vector<word> stack;
    unsigned char data[MEM_SIZE];
    std::vector<Peripheral *> peripherals;
public:
    Mem(Reg *reg) : reg(reg) {
        memset(data, 0x60, MEM_SIZE * sizeof(unsigned char));
    };
    word translate(word, Mode);
    word load(word, Mode);
    void store(word, word, Mode);
    void push(word);
    bool stackEmpty() { return stack.size() == 0; }
    word pop();
    void copyTo(std::string, word);
    std::string dump();
    void connect(Peripheral *p) { peripherals.push_back(p); }
    void broadcast();
};

#endif
