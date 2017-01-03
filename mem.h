#ifndef MEM_H
#define MEM_H

#include <string>
#include <vector>

#include "types.h"
#include "reg.h"
#include "peripheral.h"

#define MEM_SIZE 0x10000

enum Mode{IMM, ABS, ZERO, IND_ABS, ABSX,
        ABSY, ZEROX, ZEROY, IDX_IND,
        IND_IDX, REL, ACC, VOID};

extern std::string modeName[13];

class Mem {
    Reg &reg;
    std::vector<word> stack;
    unsigned char data[MEM_SIZE];
    std::vector<Peripheral> peripherals;
public:
    Mem(Reg &reg) : reg(reg) {};
    word translate(word, Mode);
    word load(word, Mode);
    void store(word, word, Mode);
    void push(word);
    bool stackEmpty() { return stack.size() > 0; }
    word pop();
    void copyTo(std::string, word);
private:
    void findPeripheral(word);
};

#endif
