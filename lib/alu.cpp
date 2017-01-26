#include "headers/alu.h"
#include "headers/log.h"

void wrapup(word operand, Reg *reg) {
    reg->unsetStatus(N);
    reg->unsetStatus(Z);
    if(GET_BIT(operand.uw, 7))
        reg->setStatus(N);

    if(operand.uw == 0)
        reg->setStatus(Z);
}

void ORA(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->A.uw |= mem->load(addr, mode).uw;
    wrapup(reg->A, reg);
}

void AND(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->A.uw &= mem->load(addr, mode).uw;
    wrapup(reg->A, reg);
}

void EOR(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->A.uw ^= mem->load(addr, mode).uw;
    wrapup(reg->A, reg);
}

void ADC(word addr, Mode mode, Mem *mem, Reg *reg) { //Add
    if(reg->getStatus(D)) { // Decimal
        throw "DECIMAL NOT SUPPORTED YET";
    } else {
        unsigned char carry = reg->getStatus(C) ? 1 : 0;
        word a = reg->A;
        word b = mem->load(addr, mode);
        const unsigned short res = a.uw + b.uw + carry;

        if(res > 0xff) {
            reg->setStatus(C);
        } else {
            reg->unsetStatus(C);
        }

        reg->A.udw = res;

        const unsigned short overflow = ~(a.udw ^ b.udw) & (a.udw ^ res) & 0x80;
        if(overflow != 0) {
            reg->setStatus(V);
        } else{
            reg->unsetStatus(V);
        }

        wrapup(reg->A, reg);
    }
}

void SBC(word addr, Mode mode, Mem *mem, Reg *reg) { //Subtract
    if(reg->getStatus(D)) { // Decimal
        throw "DECIMAL NOT SUPPORTED YET";
    } else {
        unsigned char carry = reg->getStatus(C) ? 1 : 0;
        word a = reg->A;
        word b = mem->load(addr, mode);
        b.uw = ~b.uw; // This is subtraction
        const unsigned short res = a.uw + b.uw + carry;

        if(res > 0xff) {
            reg->setStatus(C);
        } else {
            reg->unsetStatus(C);
        }

        reg->A.udw = res;

        const unsigned short overflow = ~(a.udw ^ b.udw) & (a.udw ^ res) & 0x80;
        if(overflow != 0) {
            reg->setStatus(V);
        } else{
            reg->unsetStatus(V);
        }

        wrapup(reg->A, reg);
    }
}

void STA(word addr, Mode mode, Mem *mem, Reg *reg) {
    mem->store(reg->A, addr, mode);
    if((mode == ABSY || mode == ABSX || mode == IND_IDX) && mem->cycles > 0)
        mem->cycles--;
}

void STX(word addr, Mode mode, Mem *mem, Reg *reg) {
    mem->store(reg->X, addr, mode);
    if((mode == ABSY || mode == ABSX || mode == IND_IDX) && mem->cycles > 0)
        mem->cycles--;
}

void STY(word addr, Mode mode, Mem *mem, Reg *reg) {
    mem->store(reg->Y, addr, mode);
    if((mode == ABSY || mode == ABSX || mode == IND_IDX) && mem->cycles > 0)
        mem->cycles--;
}

void LDA(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->A = mem->load(addr, mode);
    wrapup(reg->A, reg);
}

void LDX(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->X = mem->load(addr, mode);
    wrapup(reg->X, reg);
}

void LDY(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->Y = mem->load(addr, mode);
    wrapup(reg->Y, reg);
}

void INC(word addr, Mode mode, Mem *mem, Reg *reg) {
    word tmp = mem->load(addr, mode);
    tmp.uw++;
    mem->store(tmp, addr, mode);

    wrapup(tmp, reg);
}

void INX(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->X.uw++;
    wrapup(reg->X, reg);
}

void INY(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->Y.uw++;
    wrapup(reg->Y, reg);
}

void DEC(word addr, Mode mode, Mem *mem, Reg *reg) {
    word tmp = mem->load(addr, mode);
    tmp.uw--;
    mem->store(tmp, addr, mode);

    wrapup(tmp, reg);
}

void DEX(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->X.uw--;
    wrapup(reg->X, reg);
}

void DEY(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->Y.uw--;
    wrapup(reg->Y, reg);
}

void CMP(word addr, Mode mode, Mem *mem, Reg *reg) {
    word tmp = mem->load(addr, mode);
    reg->unsetStatus(N);
    reg->unsetStatus(Z);
    reg->unsetStatus(C);
    if(((reg->A.uw - tmp.uw) & 0x80) == 0x80) {
        reg->setStatus(N);
    }
    if(reg->A.uw == tmp.uw) {
        reg->setStatus(Z);
    }

    if(reg->A.uw >= tmp.uw) {
        reg->setStatus(C);
    }
}

void CPX(word addr, Mode mode, Mem *mem, Reg *reg) {
    word tmp = mem->load(addr, mode);
    reg->unsetStatus(N);
    reg->unsetStatus(Z);
    reg->unsetStatus(C);
    if(((reg->X.uw - tmp.uw) & 0x80) == 0x80) {
        reg->setStatus(N);
    }
    if(reg->X.uw == tmp.uw) {
        reg->setStatus(Z);
    }

    if(reg->X.uw >= tmp.uw) {
        reg->setStatus(C);
    }
}

void CPY(word addr, Mode mode, Mem *mem, Reg *reg) {
    word tmp = mem->load(addr, mode);
    reg->unsetStatus(N);
    reg->unsetStatus(Z);
    reg->unsetStatus(C);
    if(((reg->Y.uw - tmp.uw) & 0x80) == 0x80) {
        reg->setStatus(N);
    }
    if(reg->Y.uw == tmp.uw) {
        reg->setStatus(Z);
    }

    if(reg->Y.uw >= tmp.uw) {
        reg->setStatus(C);
    }
}

void BIT(word addr, Mode mode, Mem *mem, Reg *reg) {
    word tmp = mem->load(addr, mode);

    if(GET_BIT(tmp.uw, 7)) {
        reg->setStatus(N);
    } else {
        reg->unsetStatus(N);
    }

    if(GET_BIT(tmp.uw, 6)) {
        reg->setStatus(V);
    } else {
        reg->unsetStatus(V);
    }

    tmp.uw &= reg->A.uw;
    if(tmp.uw == 0)
        reg->setStatus(Z);
}

void ASL(word addr, Mode mode, Mem *mem, Reg *reg) {
    word tmp;
    try {
        tmp = mem->load(addr, mode);
    } catch(Mode m) {
        if(m != ACC)
            throw "Invalid Instruction";

        tmp = reg->A;
    }
    if(GET_BIT(tmp.uw, 7)) {
        reg->setStatus(C);
    } else {
        reg->unsetStatus(C);
    }

    tmp.uw <<= 1;
    if(mode == ACC) {
        reg->A.uw = tmp.uw;
    } else {
        mem->store(tmp, addr, mode);
    }

    wrapup(tmp, reg);
}

void LSR(word addr, Mode mode, Mem *mem, Reg *reg) {
    word tmp;
    try {
        tmp = mem->load(addr, mode);
    } catch(Mode m) {
        if(m != ACC)
            throw "Invalid Instruction";

        tmp = reg->A;
    }
    if(GET_BIT(tmp.uw, 0)) {
        reg->setStatus(C);
    } else {
        reg->unsetStatus(C);
    }

    tmp.uw >>= 1;
    if(mode == ACC) {
        reg->A.uw = tmp.uw;
    } else {
        mem->store(tmp, addr, mode);
    }

    wrapup(tmp, reg);
}

void ROL(word addr, Mode mode, Mem *mem, Reg *reg) {
    if(reg->getStatus(C)) {
        word tmp;
        try {
            tmp = mem->load(addr, mode);
        } catch(Mode m) {
            if(m != ACC)
                throw "Invalid Instruction";

            tmp = reg->A;
        }
        if(GET_BIT(tmp.uw, 7)) {
            reg->setStatus(C);
        } else {
            reg->unsetStatus(C);
        }
        tmp.uw <<= 1;
        tmp.uw |= 1;
        if(mode == ACC) {
            reg->A.uw = tmp.uw;
        } else {
            mem->store(tmp, addr, mode);
        }
        wrapup(tmp, reg);
    } else {
        ASL(addr, mode, mem, reg);
    }
}

void ROR(word addr, Mode mode, Mem *mem, Reg *reg) {
    if(reg->getStatus(C)) {
        word tmp;
        try {
            tmp = mem->load(addr, mode);
        } catch(Mode m) {
            if(m != ACC)
                throw "Invalid Instruction";

            tmp = reg->A;
        }
        if(GET_BIT(tmp.uw, 0)) {
            reg->setStatus(C);
        } else {
            reg->unsetStatus(C);
        }
        tmp.uw >>= 1;
        tmp.uw |= 128;
        if(mode == ACC) {
            reg->A.uw = tmp.uw;
        } else {
            mem->store(tmp, addr, mode);
        }
        wrapup(tmp, reg);
    } else {
        LSR(addr, mode, mem, reg);
    }
}

void TAX(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->X = reg->A;
    wrapup(reg->X, reg);
}

void TAY(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->Y = reg->A;
    wrapup(reg->Y, reg);
}

void TXA(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->A = reg->X;
    wrapup(reg->A, reg);
}

void TYA(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->A = reg->Y;
    wrapup(reg->Y, reg);
}

void TSX(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->X = reg->S;
    wrapup(reg->X, reg);
}

void TXS(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->S = reg->X;
}

void PHA(word addr, Mode mode, Mem *mem, Reg *reg) {
    mem->push8(reg->A);
}

void PHP(word addr, Mode mode, Mem *mem, Reg *reg) {
    mem->push8(reg->P);
}

void PLA(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->A = mem->pop8();
    wrapup(reg->A, reg);
}

void PLP(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->P = mem->pop8();
}

void CLC(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->unsetStatus(C);
}

void CLD(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->unsetStatus(D);
}

void CLI(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->unsetStatus(I);
}

void CLV(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->unsetStatus(V);
}

void SEC(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->setStatus(C);
}

void SED(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->setStatus(D);
}

void SEI(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->setStatus(I);
}

void NOP(word addr, Mode mode, Mem *mem, Reg *reg) {}

void BRK(word addr, Mode mode, Mem *mem, Reg *reg) {
    word tmp;
    tmp.udw = 0xfffe;
    reg->PC.upart.hi = mem->load(tmp, ABS).uw;
    tmp.udw = 0xffff;
    reg->PC.upart.lo = mem->load(tmp, ABS).uw;
}

void JMP(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->PC = mem->translate(addr, mode);
}

void BCC(word addr, Mode mode, Mem *mem, Reg *reg) {
    if(!reg->getStatus(C)) {
        mem->cycles++; // Hack to make branches work
        word tmp = mem->translate(addr, mode);
        if(!mem->pagesEqual(tmp, reg->PC))
            mem->cycles++;
        reg->PC = tmp;
    }
}

void BCS(word addr, Mode mode, Mem *mem, Reg *reg) {
    if(reg->getStatus(C)) {
        mem->cycles++; // Hack to make branches work
        word tmp = mem->translate(addr, mode);
        if(!mem->pagesEqual(tmp, reg->PC))
            mem->cycles++;
        reg->PC = tmp;
    }
}

void BEQ(word addr, Mode mode, Mem *mem, Reg *reg) {
    if(reg->getStatus(Z)) {
        mem->cycles++; // Hack to make branches work
        word tmp = mem->translate(addr, mode);
        if(!mem->pagesEqual(tmp, reg->PC))
            mem->cycles++;
        reg->PC = tmp;
    }
}

void BNE(word addr, Mode mode, Mem *mem, Reg *reg) {
    if(!reg->getStatus(Z)) {
        mem->cycles++; // Hack to make branches work
        word tmp = mem->translate(addr, mode);
        if(!mem->pagesEqual(tmp, reg->PC))
            mem->cycles++;
        reg->PC = tmp;
    }
}

void BMI(word addr, Mode mode, Mem *mem, Reg *reg) {
    if(reg->getStatus(N)) {
        mem->cycles++; // Hack to make branches work
        word tmp = mem->translate(addr, mode);
        if(!mem->pagesEqual(tmp, reg->PC))
            mem->cycles++;
        reg->PC = tmp;
    }
}

void BPL(word addr, Mode mode, Mem *mem, Reg *reg) {
    if(!reg->getStatus(N)) {
        mem->cycles++; // Hack to make branches work
        word tmp = mem->translate(addr, mode);
        if(!mem->pagesEqual(tmp, reg->PC))
            mem->cycles++;
        reg->PC = tmp;
    }
}

void BVS(word addr, Mode mode, Mem *mem, Reg *reg) {
    if(reg->getStatus(V)) {
        mem->cycles++; // Hack to make branches work
        word tmp = mem->translate(addr, mode);
        if(!mem->pagesEqual(tmp, reg->PC))
            mem->cycles++;
        reg->PC = tmp;
    }
}

void BVC(word addr, Mode mode, Mem *mem, Reg *reg) {
    if(!reg->getStatus(V)) {
        mem->cycles++; // Hack to make branches work
        word tmp = mem->translate(addr, mode);
        if(!mem->pagesEqual(tmp, reg->PC))
            mem->cycles++;
        reg->PC = tmp;
    }
}

void JSR(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->PC.udw -= 1;
    mem->push16(reg->PC);
    reg->PC = mem->translate(addr, mode);
}

void RTS(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->PC = mem->pop16();
    reg->PC.udw += 1;
}

void RTI(word addr, Mode mode, Mem *mem, Reg *reg) {
    PLP(addr, mode, mem, reg);
    reg->PC = mem->pop16();
}
