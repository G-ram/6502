#include "headers/decoder.h"
#include "headers/log.h"

std::ostream& operator<<(std::ostream &out, const Decoder::Instruct &i) {
    out << "OP(" << i.name << ") M(" << modeName[i.mode] << ")" << " AD(" << hexify(i.addr.udw) << ")";
    return out;
}

Decoder::Instruct Decoder::decode() {
    word raw, zero;
    raw.uw = mem->load(reg->PC, ABS).uw;
    reg->PC.udw++;
    unsigned char baseCycles = instructionCycles[raw.uw];
    unsigned char pageCycles = instructionPageCycles[raw.uw];

    switch(raw.uw) {
        case 0x10: return Decoder::Instruct("BPL", BPL, REL, nextByte(), baseCycles, pageCycles);
        case 0x30: return Decoder::Instruct("BMI", BMI, REL, nextByte(), baseCycles, pageCycles);
        case 0x50: return Decoder::Instruct("BVC", BVC, REL, nextByte(), baseCycles, pageCycles);
        case 0x70: return Decoder::Instruct("BVS", BVS, REL, nextByte(), baseCycles, pageCycles);
        case 0x90: return Decoder::Instruct("BCC", BCC, REL, nextByte(), baseCycles, pageCycles);
        case 0xb0: return Decoder::Instruct("BCS", BCS, REL, nextByte(), baseCycles, pageCycles);
        case 0xd0: return Decoder::Instruct("BNE", BNE, REL, nextByte(), baseCycles, pageCycles);
        case 0xf0: return Decoder::Instruct("BEQ", BEQ, REL, nextByte(), baseCycles, pageCycles);
        case 0x20:
        {
            word tmp = next2Bytes();
            return Decoder::Instruct("JSR", JSR, ABS, tmp, baseCycles, pageCycles);
        }
        case 0x40: return Decoder::Instruct("RTI", RTI, VOID, zero, baseCycles, pageCycles);
        case 0x60: return Decoder::Instruct("RTS", RTS, VOID, zero, baseCycles, pageCycles);
        case 0x08: return Decoder::Instruct("PHP", PHP, VOID, zero, baseCycles, pageCycles);
        case 0x28: return Decoder::Instruct("PLP", PLP, VOID, zero, baseCycles, pageCycles);
        case 0x48: return Decoder::Instruct("PHA", PHA, VOID, zero, baseCycles, pageCycles);
        case 0x68: return Decoder::Instruct("PLA", PLA, VOID, zero, baseCycles, pageCycles);
        case 0x88: return Decoder::Instruct("DEY", DEY, VOID, zero, baseCycles, pageCycles);
        case 0xA8: return Decoder::Instruct("TAY", TAY, VOID, zero, baseCycles, pageCycles);
        case 0xC8: return Decoder::Instruct("INY", INY, VOID, zero, baseCycles, pageCycles);
        case 0xE8: return Decoder::Instruct("INX", INX, VOID, zero, baseCycles, pageCycles);
        case 0x18: return Decoder::Instruct("CLC", CLC, VOID, zero, baseCycles, pageCycles);
        case 0x38: return Decoder::Instruct("SEC", SEC, VOID, zero, baseCycles, pageCycles);
        case 0x58: return Decoder::Instruct("CLI", CLI, VOID, zero, baseCycles, pageCycles);
        case 0x78: return Decoder::Instruct("SEI", SEI, VOID, zero, baseCycles, pageCycles);
        case 0x98: return Decoder::Instruct("TYA", TYA, VOID, zero, baseCycles, pageCycles);
        case 0xB8: return Decoder::Instruct("CLV", CLV, VOID, zero, baseCycles, pageCycles);
        case 0xD8: return Decoder::Instruct("CLD", CLD, VOID, zero, baseCycles, pageCycles);
        case 0xF8: return Decoder::Instruct("SED", SED, VOID, zero, baseCycles, pageCycles);
        case 0x8A: return Decoder::Instruct("TXA", TXA, VOID, zero, baseCycles, pageCycles);
        case 0x9A: return Decoder::Instruct("TXS", TXS, VOID, zero, baseCycles, pageCycles);
        case 0xAA: return Decoder::Instruct("TAX", TAX, VOID, zero, baseCycles, pageCycles);
        case 0xBA: return Decoder::Instruct("TSX", TSX, VOID, zero, baseCycles, pageCycles);
        case 0xCA: return Decoder::Instruct("DEX", DEX, VOID, zero, baseCycles, pageCycles);
        case 0x00: return Decoder::Instruct("BRK", BRK, VOID, zero, baseCycles, pageCycles);
        case 0xEA: return Decoder::Instruct("NOP", NOP, VOID, zero, baseCycles, pageCycles);
        default:
        {
            unsigned char a = (raw.uw >> 5) & 7;
            unsigned char b = (raw.uw >> 2) & 7;
            unsigned char c = raw.uw & 3;
            switch(c) {
                case 1:
                    if(maskRaw1[b] == 2) {
                        return Decoder::Instruct(maskName1[a], maskOp1[a], maskMode1[b], next2Bytes(), baseCycles, pageCycles);
                    } else {
                        return Decoder::Instruct(maskName1[a], maskOp1[a], maskMode1[b], nextByte(), baseCycles, pageCycles);
                    }
                case 2:
                    if(maskRaw2[b] == 2) {
                        if(maskOp2[a] == LDX && maskMode2[b] == ABSX)
                            return Decoder::Instruct(maskName2[a], maskOp2[a], ABSY, next2Bytes(), baseCycles, pageCycles);

                        return Decoder::Instruct(maskName2[a], maskOp2[a], maskMode2[b], next2Bytes(), baseCycles, pageCycles);
                    } else if(maskRaw2[b] == 1){
                        if((maskOp2[a] == STX || maskOp2[a] == LDX) && maskMode2[b] == ZEROX)
                            return Decoder::Instruct(maskName2[a], maskOp2[a], ZEROY, nextByte(), baseCycles, pageCycles);

                        return Decoder::Instruct(maskName2[a], maskOp2[a], maskMode2[b], nextByte(), baseCycles, pageCycles);
                    }
                    return Decoder::Instruct(maskName2[a], maskOp2[a], maskMode2[b], zero, baseCycles, pageCycles);
                case 0:
                    if(maskRaw0[b] == 2) {
                        if(a == 3) {
                            return Decoder::Instruct(maskName0[a], maskOp0[a], IND_ABS, next2Bytes(), baseCycles, pageCycles);
                        }
                        return Decoder::Instruct(maskName0[a], maskOp0[a], maskMode0[b], next2Bytes(), baseCycles, pageCycles);
                    } else if(maskRaw0[b] == 1){
                        return Decoder::Instruct(maskName0[a], maskOp0[a], maskMode0[b], nextByte(), baseCycles, pageCycles);
                    }
                    return Decoder::Instruct(maskName0[a], maskOp0[a], maskMode0[b], zero, baseCycles, pageCycles);
                default:
                    GLOG("PC: " << hexify(reg->PC.udw) << " " << binify(raw.uw));
                    throw "Not a valid Decoder::Instruction";
            }
        }
    }
    return Decoder::Instruct("NOP", NOP, VOID, zero, baseCycles, pageCycles);
}

bool Decoder::hasNext() {
    if(mem->load(reg->PC, ABS).uw == 0x00) { // BRK
        reg->setStatus(B);
        return false;
    }

    return true;
}

word Decoder::next2Bytes() {
    word tmp;
    tmp.upart.lo = mem->load(reg->PC, ABS).uw;
    reg->PC.udw++;
    tmp.upart.hi = mem->load(reg->PC, ABS).uw;
    reg->PC.udw++;
    return tmp;
}

word Decoder::nextByte() {
    word tmp;
    tmp.uw = mem->load(reg->PC, ABS).uw;
    reg->PC.udw++;
    return tmp;
}
