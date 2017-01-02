#ifndef INSTRUCT_H
#define INSTRUCT_H

#include "types.h"
#include "mem.h"
#include "reg.h"
#include "alu.h"

typedef func (void (*)(word, Mode, Mem &, Reg &))

class Decoder{
private:
    func maskOp1 = {ORA, AND, EOR, ADC, STA, LDA, CMP, SBC};
    Mode maskMode1 = {IDX_IND, ZERO, IMM, ABS, IND_IDX, ZEROX, ABSY, ABSX};
    char maskRaw1 = {1, 1, 1, 2, 1, 1, 2, 2};

    func maskOp2 = {ASL, ROL, LSR, ROR, STX, LDX, DEC, INC};
    Mode maskMode2 = {IMM, ZERO, ACC, ABS, VOID, ZEROX, VOID, ABSX};
    char maskRaw2 = {1, 1, 0, 2, 0, 1, 0, 2};

    func maskOp0 = {NOP, BIT, JMP, JMP, STY, LDY, CPY, CPX};
    Mode maskMode0 = {IMM, ZERO, VOID, ABS, VOID, ZEROX, VOID, ABSX};
    char maskRaw0 = {1, 1, 0, 2, 0, 1, 0, 2};
public:
    struct Instruct {
        func op;
        Mode mode;
        word addr;
    };
public:
    Decoder() {};
    Instruct decode(Mem &, Reg &);
    bool hasNext();
private:
    word next2Bytes(Reg &reg);
    word nextByte(Reg &reg);
};

#endif
