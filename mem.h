#ifndef MEM_H
#define MEM_H

#include <string>
#include <vector>

#include "types.h"
#include "reg.h"
#include "peripheral.h"

enum Mode{IMM, ABS, ZERO, IND_ABS, ABSX,
        ABSY, ZEROX, ZEROY, IDX_IND,
        IND_IDX, REL, ACC, VOID};

class Mem {
    Reg &reg;
    size_t size;
    std::vector<word> stack;
    std::vector<char> data;
    std::vector<Peripheral> peripherals;
public:
    Mem(size_t size, Reg &reg) : size(size), reg(reg) {
        data.reserve(size);
    };
    word translate(word, Mode);
    word load(word, Mode);
    void store(word, word, Mode);
    void push(word);
    void stackEmpty() { return stack.size() > 0; }
    word pop(word);
    void copyTo(std::string, word);
private:
    void findPeripheral(word);
};
#endif
