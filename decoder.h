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
        Instruct(std::string name, func op, Mode mode, word addr)
            : name(name), op(op), mode(mode), addr(addr) {};
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
