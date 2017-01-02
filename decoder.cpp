#include "decoder.h"
#include "types.h"

Instruct Decoder::decode(Mem &mem, Reg &reg) {
    word raw, zero;
    raw.uw = mem.load(reg.PC, ABS).uw;
    reg.PC.udw++;

    switch(raw.uw) {
        case 0x10: return Instruct(BPL, VOID, zero);
        case 0x30: return Instruct(BMI, VOID, zero);
        case 0x50: return Instruct(BVC, VOID, zero);
        case 0x70: return Instruct(BVS, VOID, zero);
        case 0x90: return Instruct(BCC, VOID, zero);
        case 0xb0: return Instruct(BCS, VOID, zero);
        case 0xd0: return Instruct(BNE, VOID, zero);
        case 0xf0: return Instruct(BEQ, VOID, zero);
        case 0x00: return Instruct(BRK, VOID, zero);
        case 0x20:
            std::string tmp = next2Bytes(reg);
            return Instruct(JSR, VOID, tmp);
        case 0x40: return Instruct(RTI, VOID, zero);
        case 0x60: return Instruct(RTS, VOID, zero);
        case 0x08: return Instruct(PHP, VOID, zero);
        case 0x28: return Instruct(PLP, VOID, zero);
        case 0x48: return Instruct(PHA, VOID, zero);
        case 0x88: return Instruct(DEY, VOID, zero);
        case 0xA8: return Instruct(TAY, VOID, zero);
        case 0xC8: return Instruct(INY, VOID, zero);
        case 0xE8: return Instruct(INX, VOID, zero);
        case 0x18: return Instruct(CLC, VOID, zero);
        case 0x38: return Instruct(SEC, VOID, zero);
        case 0x58: return Instruct(CLI, VOID, zero);
        case 0x78: return Instruct(SEI, VOID, zero);
        case 0x98: return Instruct(TYA, VOID, zero);
        case 0xB8: return Instruct(CLV, VOID, zero);
        case 0xD8: return Instruct(CLD, VOID, zero);
        case 0xF8: return Instruct(SED, VOID, zero);
        case 0x8A: return Instruct(TXA, VOID, zero);
        case 0x9A: return Instruct(TXS, VOID, zero);
        case 0xAA: return Instruct(TAX, VOID, zero);
        case 0xBA: return Instruct(TSX, VOID, zero);
        case 0xCA: return Instruct(DEX, VOID, zero);
        case 0xEA: return Instruct(NOP, VOID, zero);
        default:
            char a = (raw.uw >> 5) && 7;
            char b = (raw.uw >> 2) && 7;
            char c = raw.uw && 3;
            switch(c) {
                case 1:
                    if(maskRaw1[b] == 2) {
                        return Instruct(maskOp1[a], maskMode1[b], next2Bytes(reg));
                    } else {
                        return Instruct(maskOp1[a], maskMode1[b], nextByte(reg));
                    }
                case 2:
                    if(maskRaw2[b] == 2) {
                        return Instruct(maskOp2[a], maskMode2[b], next2Bytes(reg));
                    } else if(maskRaw2[b] == 1){
                        return Instruct(maskOp2[a], maskMode2[b], nextByte(reg));
                    }
                    return Instruct(maskOp2[a], maskMode2[b], zero);
                case 0:
                    if(maskRaw0[b] == 2) {
                        return Instruct(maskOp0[a], maskMode0[b], next2Bytes(reg));
                    } else if(maskRaw0[b] == 1){
                        return Instruct(maskOp2[a], maskMode0[b], nextByte(reg));
                    }
                    return Instruct(maskOp0[a], maskMode0[b], zero);
                default: throw "Not a valid instruction";
            }
    }
    return Instruct(NOP, VOID, zero);
}

bool hasNext() {
    if(mem.load(reg.PC, ABS).uw == 0x60 && mem.stackEmpty()) // RTS and stack is empty
        return false;

    return true;
}

word Decoder::next2Bytes(Reg &reg) {
    word tmp;
    tmp.upart.lo = mem.load(reg.PC, ABS).uw;
    reg.PC.udw++;
    tmp.upart.hi = mem.load(reg.PC, ABS).uw;
    reg.PC.udw++;
    return tmp;
}

word Decoder::nextByte(Reg &reg) {
    word tmp;
    tmp.uw = mem.load(reg.PC, ABS).uw;
    reg.PC.udw++;
    return tmp;
}
