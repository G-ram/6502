#include "alu.h"
#include "log.h"

void wrapup(word operand, Reg *reg) {
    if(GET_BIT(operand.uw, 7))
        reg->setStatus(N);

    if(operand.uw == 0)
        reg->setStatus(Z);
}

void ORA(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->A.w |= mem->load(addr, mode).w;
    wrapup(reg->A, reg);
}

void AND(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->A.w &= mem->load(addr, mode).w;
    wrapup(reg->A, reg);
}

void EOR(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->A.w ^= mem->load(addr, mode).w;
    wrapup(reg->A, reg);
}

void ADC(word addr, Mode mode, Mem *mem, Reg *reg) { //Add
    if(reg->getStatus(D)) { // Decimal
        throw "DECIMAL NOT SUPPORTED YET";
    } else {
        bool sign = GET_BIT(reg->A.uw, 7);
        char carry = reg->getStatus(C) ? 1 : 0;
        word a = reg->A;
        word b = mem->load(addr, mode);
        word tmp;
        tmp.uw = a.uw + b.uw + carry;

        if(a.w > 0 && b.w > 0 && tmp.w < 0) {
            reg->setStatus(C);
        }
        reg->A.udw = tmp.udw;

        if(sign != GET_BIT(reg->A.w, 7)) {
            reg->setStatus(V);
        }

        wrapup(reg->A, reg);
    }
}

void SBC(word addr, Mode mode, Mem *mem, Reg *reg) { //Subtract
    if(reg->getStatus(D)) { // Decimal
        throw "DECIMAL NOT SUPPORTED YET";
    } else {
        bool sign = GET_BIT(reg->A.uw, 7);
        char carry = reg->getStatus(C) ? 0 : 1;
        word a = reg->A;
        word b = mem->load(addr, mode);
        word tmp;
        tmp.uw = a.uw - b.uw - carry;

        if(a.w < 0 && b.w < 0 && tmp.w > 0) {
            reg->setStatus(C);
        }
        reg->A.udw = tmp.udw;

        if(sign != GET_BIT(reg->A.w, 7)) {
            reg->setStatus(V);
        }

        wrapup(reg->A, reg);
    }
}

void STA(word addr, Mode mode, Mem *mem, Reg *reg) {
    mem->store(reg->A, addr, mode);
}

void STX(word addr, Mode mode, Mem *mem, Reg *reg) {
    mem->store(reg->X, addr, mode);
}

void STY(word addr, Mode mode, Mem *mem, Reg *reg) {
    mem->store(reg->Y, addr, mode);
}

void LDA(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->A.w = mem->load(addr, mode).w;
    wrapup(reg->A, reg);
}

void LDX(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->X.w = mem->load(addr, mode).w;
    wrapup(reg->X, reg);
}

void LDY(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->Y.w = mem->load(addr, mode).w;
    wrapup(reg->Y, reg);
}

void INC(word addr, Mode mode, Mem *mem, Reg *reg) {
    word tmp = mem->load(addr, mode);
    tmp.w++;
    mem->store(tmp, addr, mode);

    wrapup(tmp, reg);
}

void INX(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->X.w++;
    wrapup(reg->X, reg);
}

void INY(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->Y.w++;
    wrapup(reg->Y, reg);
}

void DEC(word addr, Mode mode, Mem *mem, Reg *reg) {
    word tmp = mem->load(addr, mode);
    tmp.w--;
    mem->store(tmp, addr, mode);

    wrapup(tmp, reg);
}

void DEX(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->X.w--;
    wrapup(reg->X, reg);
}

void DEY(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->Y.w--;
    wrapup(reg->Y, reg);
}

void CMP(word addr, Mode mode, Mem *mem, Reg *reg) {
    word tmp = mem->load(addr, mode);
    if(reg->A.w < tmp.w) {
        reg->setStatus(N);
        reg->unsetStatus(Z);
        reg->unsetStatus(C);
    } else if(reg->A.w == tmp.w) {
        reg->unsetStatus(N);
        reg->setStatus(Z);
        reg->setStatus(C);
    } else {
        reg->unsetStatus(N);
        reg->unsetStatus(Z);
        reg->setStatus(C);
    }
}

void CPX(word addr, Mode mode, Mem *mem, Reg *reg) {
    word tmp = mem->load(addr, mode);
    if(reg->X.w < tmp.w) {
        reg->setStatus(N);
        reg->unsetStatus(Z);
        reg->unsetStatus(C);
    } else if(reg->X.w == tmp.w) {
        reg->unsetStatus(N);
        reg->setStatus(Z);
        reg->setStatus(C);
    } else {
        reg->unsetStatus(N);
        reg->unsetStatus(Z);
        reg->setStatus(C);
    }
}

void CPY(word addr, Mode mode, Mem *mem, Reg *reg) {
    word tmp = mem->load(addr, mode);
    if(reg->Y.w < tmp.w) {
        reg->setStatus(N);
        reg->unsetStatus(Z);
        reg->unsetStatus(C);
    } else if(reg->Y.w == tmp.w) {
        reg->unsetStatus(N);
        reg->setStatus(Z);
        reg->setStatus(C);
    } else {
        reg->unsetStatus(N);
        reg->unsetStatus(Z);
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

    tmp.w &= reg->A.w;
    if(tmp.uw == 0)
        reg->setStatus(Z);
}

void ASL(word addr, Mode mode, Mem *mem, Reg *reg) {
    word tmp = mem->load(addr, mode);
    if(GET_BIT(tmp.w, 7)) {
        reg->setStatus(C);
    } else {
        reg->unsetStatus(C);
    }

    tmp.w <<= 1;
    mem->store(tmp, addr, mode);

    wrapup(tmp, reg);
}

void LSR(word addr, Mode mode, Mem *mem, Reg *reg) {
    word tmp = mem->load(addr, mode);
    if(GET_BIT(tmp.w, 0)) {
        reg->setStatus(C);
    } else {
        reg->unsetStatus(C);
    }

    tmp.w >>= 1;
    mem->store(tmp, addr, mode);

    wrapup(tmp, reg);
}

void ROL(word addr, Mode mode, Mem *mem, Reg *reg) {
    word tmp = mem->load(addr, mode);
    if(reg->getStatus(C)) {
        if(GET_BIT(tmp.uw, 7)) {
            reg->setStatus(C);
        } else {
            reg->unsetStatus(C);
        }
        tmp.w <<= 1;
        tmp.w |= 1;
        wrapup(tmp, reg);
    } else {
        ASL(addr, mode, mem, reg);
    }
}

void ROR(word addr, Mode mode, Mem *mem, Reg *reg) {
    word tmp = mem->load(addr, mode);
    if(reg->getStatus(C)) {
        if(GET_BIT(tmp.uw, 0)) {
            reg->setStatus(C);
        } else {
            reg->unsetStatus(C);
        }
        tmp.w >>= 1;
        tmp.w |= 128;
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
    reg->Y = reg->X;
    wrapup(reg->Y, reg);
}

void TSX(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->P = reg->X;
}

void TXS(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->X = reg->P;
}

void PHA(word addr, Mode mode, Mem *mem, Reg *reg) {
    mem->push(reg->A);
}

void PHP(word addr, Mode mode, Mem *mem, Reg *reg) {
    mem->push(reg->S);
}

void PLA(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->A = mem->pop();
}

void PLP(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->S = mem->pop();
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
        reg->PC = mem->translate(addr, mode);
    }
}

void BCS(word addr, Mode mode, Mem *mem, Reg *reg) {
    if(reg->getStatus(C)) {
        reg->PC = mem->translate(addr, mode);
    }
}

void BEQ(word addr, Mode mode, Mem *mem, Reg *reg) {
    if(reg->getStatus(Z)) {
        reg->PC = mem->translate(addr, mode);
    }
}

void BNE(word addr, Mode mode, Mem *mem, Reg *reg) {
    if(!reg->getStatus(Z)) {
        reg->PC = mem->translate(addr, mode);
    }
}

void BMI(word addr, Mode mode, Mem *mem, Reg *reg) {
    if(reg->getStatus(N)) {
        reg->PC = mem->translate(addr, mode);
    }
}

void BPL(word addr, Mode mode, Mem *mem, Reg *reg) {
    if(!reg->getStatus(N)) {
        reg->PC = mem->translate(addr, mode);
    }
}

void BVS(word addr, Mode mode, Mem *mem, Reg *reg) {
    if(reg->getStatus(V)) {
        reg->PC = mem->translate(addr, mode);
    }
}

void BVC(word addr, Mode mode, Mem *mem, Reg *reg) {
    if(!reg->getStatus(V)) {
        reg->PC = mem->translate(addr, mode);
    }
}

void JSR(word addr, Mode mode, Mem *mem, Reg *reg) {
    mem->push(addr);
    reg->PC = mem->translate(addr, mode);
}

void RTS(word addr, Mode mode, Mem *mem, Reg *reg) {
    reg->PC = mem->pop();
}

void RTI(word addr, Mode mode, Mem *mem, Reg *reg) {
    RTS(addr, mode, mem, reg);
}
