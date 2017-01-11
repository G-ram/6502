#ifndef INSTRUCT_H
#define INSTRUCT_H

#include <string>
#include <iostream>

#include "types.h"
#include "mem.h"
#include "reg.h"
#include "alu.h"

typedef void (*func)(word, Mode, Mem *, Reg *);

class Decoder{
private:
    unsigned char instructionCycles[256] = {
    	7, 6, 2, 8, 3, 3, 5, 5, 3, 2, 2, 2, 4, 4, 6, 6,
    	2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
    	6, 6, 2, 8, 3, 3, 5, 5, 4, 2, 2, 2, 4, 4, 6, 6,
    	2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
    	6, 6, 2, 8, 3, 3, 5, 5, 3, 2, 2, 2, 3, 4, 6, 6,
    	2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
    	6, 6, 2, 8, 3, 3, 5, 5, 4, 2, 2, 2, 5, 4, 6, 6,
    	2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
    	2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4,
    	2, 6, 2, 6, 4, 4, 4, 4, 2, 5, 2, 5, 5, 5, 5, 5,
    	2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4,
    	2, 5, 2, 5, 4, 4, 4, 4, 2, 4, 2, 4, 4, 4, 4, 4,
    	2, 6, 2, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6,
    	2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
    	2, 6, 2, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6,
    	2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
    };

    unsigned char instructionPageCycles[256] = {
    	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    	1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0,
    	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    	1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0,
    	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    	1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0,
    	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    	1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0,
    	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    	1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1,
    	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    	1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0,
    	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    	1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0,
    };

    func maskOp1[8] = {ORA, AND, EOR, ADC, STA, LDA, CMP, SBC};
    std::string maskName1[8] = {"ORA", "AND", "EOR", "ADC", "STA", "LDA", "CMP", "SBC"};
    Mode maskMode1[8] = {IDX_IND, ZERO, IMM, ABS, IND_IDX, ZEROX, ABSY, ABSX};
    char maskRaw1[8] = {1, 1, 1, 2, 1, 1, 2, 2};

    func maskOp2[8] = {ASL, ROL, LSR, ROR, STX, LDX, DEC, INC};
    std::string maskName2[8] = {"ASL", "ROL", "LSR", "ROR", "STX", "LDX", "DEC", "INC"};
    Mode maskMode2[8] = {IMM, ZERO, ACC, ABS, VOID, ZEROX, VOID, ABSX};
    char maskRaw2[8] = {1, 1, 0, 2, 0, 1, 0, 2};

    func maskOp0[8] = {NOP, BIT, JMP, JMP, STY, LDY, CPY, CPX};
    std::string maskName0[8] = {"NOP", "BIT", "JMP", "JMP", "STY", "LDY", "CPY", "CPX"};
    Mode maskMode0[8] = {IMM, ZERO, VOID, ABS, IND_ABS, ZEROX, VOID, ABSX};
    char maskRaw0[8] = {1, 1, 0, 2, 0, 1, 0, 2};
private:
    Mem *mem;
    Reg *reg;
public:
    struct Instruct {
        std::string name;
        func op;
        Mode mode;
        word addr;
        unsigned char cycles;
        unsigned char pageCycles;
        Instruct(std::string name, func op, Mode mode, word addr, unsigned char cycles, unsigned char pageCycles)
            : name(name), op(op), mode(mode), addr(addr), cycles(cycles), pageCycles(pageCycles) {};
    };
public:
    Decoder(Mem *mem, Reg *reg) : mem(mem), reg(reg) {};
    Instruct decode();
    bool hasNext();
private:
    word next2Bytes();
    word nextByte();
};

std::ostream& operator<<(std::ostream &out, const Decoder::Instruct &i);

#endif
