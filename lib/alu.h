#ifndef INIT_H
#define INIT_H

#include "types.h"
#include "mem.h"
#include "reg.h"

void wrapup(word operand, Reg *);

void ORA(word, Mode, Mem *, Reg *);
void AND(word, Mode, Mem *, Reg *);
void EOR(word, Mode, Mem *, Reg *);
void ADC(word, Mode, Mem *, Reg *);
void SBC(word, Mode, Mem *, Reg *);
void STA(word, Mode, Mem *, Reg *);
void STX(word, Mode, Mem *, Reg *);
void STY(word, Mode, Mem *, Reg *);
void LDA(word, Mode, Mem *, Reg *);
void LDX(word, Mode, Mem *, Reg *);
void LDY(word, Mode, Mem *, Reg *);
void INC(word, Mode, Mem *, Reg *);
void INX(word, Mode, Mem *, Reg *);
void INY(word, Mode, Mem *, Reg *);
void DEC(word, Mode, Mem *, Reg *);
void DEX(word, Mode, Mem *, Reg *);
void DEY(word, Mode, Mem *, Reg *);
void CMP(word, Mode, Mem *, Reg *);
void CPX(word, Mode, Mem *, Reg *);
void CPY(word, Mode, Mem *, Reg *);
void BIT(word, Mode, Mem *, Reg *);
void ASL(word, Mode, Mem *, Reg *);
void LSR(word, Mode, Mem *, Reg *);
void ROL(word, Mode, Mem *, Reg *);
void ROR(word, Mode, Mem *, Reg *);
void TAX(word, Mode, Mem *, Reg *);
void TAY(word, Mode, Mem *, Reg *);
void TXA(word, Mode, Mem *, Reg *);
void TYA(word, Mode, Mem *, Reg *);
void TSX(word, Mode, Mem *, Reg *);
void TXS(word, Mode, Mem *, Reg *);
void PHA(word, Mode, Mem *, Reg *);
void PHP(word, Mode, Mem *, Reg *);
void PLA(word, Mode, Mem *, Reg *);
void PLP(word, Mode, Mem *, Reg *);
void CLC(word, Mode, Mem *, Reg *);
void CLD(word, Mode, Mem *, Reg *);
void CLI(word, Mode, Mem *, Reg *);
void CLV(word, Mode, Mem *, Reg *);
void SEC(word, Mode, Mem *, Reg *);
void SED(word, Mode, Mem *, Reg *);
void SEI(word, Mode, Mem *, Reg *);
void NOP(word, Mode, Mem *, Reg *);
void BRK(word, Mode, Mem *, Reg *);
void JMP(word, Mode, Mem *, Reg *);
void BCC(word, Mode, Mem *, Reg *);
void BCS(word, Mode, Mem *, Reg *);
void BEQ(word, Mode, Mem *, Reg *);
void BNE(word, Mode, Mem *, Reg *);
void BMI(word, Mode, Mem *, Reg *);
void BPL(word, Mode, Mem *, Reg *);
void BVS(word, Mode, Mem *, Reg *);
void BVC(word, Mode, Mem *, Reg *);
void JSR(word, Mode, Mem *, Reg *);
void RTS(word, Mode, Mem *, Reg *);
void RTI(word, Mode, Mem *, Reg *);

#endif
