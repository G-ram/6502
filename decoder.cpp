#include "decoder.h"

Decoder::Instruct Decoder::decode() {
    word raw, zero;
    raw.uw = mem.load(reg.PC, ABS).uw;
    reg.PC.udw++;

    switch(raw.uw) {
        case 0x10: return Decoder::Instruct(BPL, VOID, zero);
        case 0x30: return Decoder::Instruct(BMI, VOID, zero);
        case 0x50: return Decoder::Instruct(BVC, VOID, zero);
        case 0x70: return Decoder::Instruct(BVS, VOID, zero);
        case 0x90: return Decoder::Instruct(BCC, VOID, zero);
        case 0xb0: return Decoder::Instruct(BCS, VOID, zero);
        case 0xd0: return Decoder::Instruct(BNE, VOID, zero);
        case 0xf0: return Decoder::Instruct(BEQ, VOID, zero);
        case 0x00: return Decoder::Instruct(BRK, VOID, zero);
        case 0x20:
        {
            word tmp = next2Bytes();
            return Decoder::Instruct(JSR, VOID, tmp);
        }
        case 0x40: return Decoder::Instruct(RTI, VOID, zero);
        case 0x60: return Decoder::Instruct(RTS, VOID, zero);
        case 0x08: return Decoder::Instruct(PHP, VOID, zero);
        case 0x28: return Decoder::Instruct(PLP, VOID, zero);
        case 0x48: return Decoder::Instruct(PHA, VOID, zero);
        case 0x88: return Decoder::Instruct(DEY, VOID, zero);
        case 0xA8: return Decoder::Instruct(TAY, VOID, zero);
        case 0xC8: return Decoder::Instruct(INY, VOID, zero);
        case 0xE8: return Decoder::Instruct(INX, VOID, zero);
        case 0x18: return Decoder::Instruct(CLC, VOID, zero);
        case 0x38: return Decoder::Instruct(SEC, VOID, zero);
        case 0x58: return Decoder::Instruct(CLI, VOID, zero);
        case 0x78: return Decoder::Instruct(SEI, VOID, zero);
        case 0x98: return Decoder::Instruct(TYA, VOID, zero);
        case 0xB8: return Decoder::Instruct(CLV, VOID, zero);
        case 0xD8: return Decoder::Instruct(CLD, VOID, zero);
        case 0xF8: return Decoder::Instruct(SED, VOID, zero);
        case 0x8A: return Decoder::Instruct(TXA, VOID, zero);
        case 0x9A: return Decoder::Instruct(TXS, VOID, zero);
        case 0xAA: return Decoder::Instruct(TAX, VOID, zero);
        case 0xBA: return Decoder::Instruct(TSX, VOID, zero);
        case 0xCA: return Decoder::Instruct(DEX, VOID, zero);
        case 0xEA: return Decoder::Instruct(NOP, VOID, zero);
        default:
        {
            unsigned char a = (raw.uw >> 5) & 7;
            unsigned char b = (raw.uw >> 2) & 7;
            unsigned char c = raw.uw & 3;
            switch(c) {
                case 1:
                    if(maskRaw1[b] == 2) {
                        return Decoder::Instruct(maskOp1[a], maskMode1[b], next2Bytes());
                    } else {
                        return Decoder::Instruct(maskOp1[a], maskMode1[b], nextByte());
                    }
                case 2:
                    if(maskRaw2[b] == 2) {
                        return Decoder::Instruct(maskOp2[a], maskMode2[b], next2Bytes());
                    } else if(maskRaw2[b] == 1){
                        return Decoder::Instruct(maskOp2[a], maskMode2[b], nextByte());
                    }
                    return Decoder::Instruct(maskOp2[a], maskMode2[b], zero);
                case 0:
                    if(maskRaw0[b] == 2) {
                        return Decoder::Instruct(maskOp0[a], maskMode0[b], next2Bytes());
                    } else if(maskRaw0[b] == 1){
                        return Decoder::Instruct(maskOp2[a], maskMode0[b], nextByte());
                    }
                    return Decoder::Instruct(maskOp0[a], maskMode0[b], zero);
                default: throw "Not a valid Decoder::Instruction";
            }
        }
    }
    return Decoder::Instruct(NOP, VOID, zero);
}

bool Decoder::hasNext() {
    if(mem.load(reg.PC, ABS).uw == 0x60 && mem.stackEmpty()) // RTS and stack is empty
        return false;

    return true;
}

word Decoder::next2Bytes() {
    word tmp;
    tmp.upart.lo = mem.load(reg.PC, ABS).uw;
    reg.PC.udw++;
    tmp.upart.hi = mem.load(reg.PC, ABS).uw;
    reg.PC.udw++;
    return tmp;
}

word Decoder::nextByte() {
    word tmp;
    tmp.uw = mem.load(reg.PC, ABS).uw;
    reg.PC.udw++;
    return tmp;
}
