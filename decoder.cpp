#include "decoder.h"
#include "log.h"

std::ostream& operator<<(std::ostream &out, const Decoder::Instruct &i) {
    out << "OP(" << i.name << ") M(" << modeName[i.mode] << ")" << " AD(" << hexify(i.addr.udw) << ")";
    return out;
}

Decoder::Instruct Decoder::decode() {
    word raw, zero;
    raw.uw = mem->load(reg->PC, ABS).uw;
    reg->PC.udw++;

    switch(raw.uw) {
        case 0x10: return Decoder::Instruct("BPL", BPL, REL, nextByte());
        case 0x30: return Decoder::Instruct("BMI", BMI, REL, nextByte());
        case 0x50: return Decoder::Instruct("BVC", BVC, REL, nextByte());
        case 0x70: return Decoder::Instruct("BVS", BVS, REL, nextByte());
        case 0x90: return Decoder::Instruct("BCC", BCC, REL, nextByte());
        case 0xb0: return Decoder::Instruct("BCS", BCS, REL, nextByte());
        case 0xd0: return Decoder::Instruct("BNE", BNE, REL, nextByte());
        case 0xf0: return Decoder::Instruct("BEQ", BEQ, REL, nextByte());
        case 0x20:
        {
            word tmp = next2Bytes();
            return Decoder::Instruct("JSR", JSR, ABS, tmp);
        }
        case 0x40: return Decoder::Instruct("RTI", RTI, VOID, zero);
        case 0x60: return Decoder::Instruct("RTS", RTS, VOID, zero);
        case 0x08: return Decoder::Instruct("PHP", PHP, VOID, zero);
        case 0x28: return Decoder::Instruct("PLP", PLP, VOID, zero);
        case 0x48: return Decoder::Instruct("PHA", PHA, VOID, zero);
        case 0x68: return Decoder::Instruct("PLA", PHA, VOID, zero);
        case 0x88: return Decoder::Instruct("DEY", DEY, VOID, zero);
        case 0xA8: return Decoder::Instruct("TAY", TAY, VOID, zero);
        case 0xC8: return Decoder::Instruct("INY", INY, VOID, zero);
        case 0xE8: return Decoder::Instruct("INX", INX, VOID, zero);
        case 0x18: return Decoder::Instruct("CLC", CLC, VOID, zero);
        case 0x38: return Decoder::Instruct("SEC", SEC, VOID, zero);
        case 0x58: return Decoder::Instruct("CLI", CLI, VOID, zero);
        case 0x78: return Decoder::Instruct("SEI", SEI, VOID, zero);
        case 0x98: return Decoder::Instruct("TYA", TYA, VOID, zero);
        case 0xB8: return Decoder::Instruct("CLV", CLV, VOID, zero);
        case 0xD8: return Decoder::Instruct("CLD", CLD, VOID, zero);
        case 0xF8: return Decoder::Instruct("SED", SED, VOID, zero);
        case 0x8A: return Decoder::Instruct("TXA", TXA, VOID, zero);
        case 0x9A: return Decoder::Instruct("TXS", TXS, VOID, zero);
        case 0xAA: return Decoder::Instruct("TAX", TAX, VOID, zero);
        case 0xBA: return Decoder::Instruct("TSX", TSX, VOID, zero);
        case 0xCA: return Decoder::Instruct("DEX", DEX, VOID, zero);
        case 0x00: return Decoder::Instruct("BRK", BRK, VOID, zero);
        case 0xEA: return Decoder::Instruct("NOP", NOP, VOID, zero);
        default:
        {
            unsigned char a = (raw.uw >> 5) & 7;
            unsigned char b = (raw.uw >> 2) & 7;
            unsigned char c = raw.uw & 3;
            switch(c) {
                case 1:
                    if(maskRaw1[b] == 2) {
                        return Decoder::Instruct(maskName1[a], maskOp1[a], maskMode1[b], next2Bytes());
                    } else {
                        return Decoder::Instruct(maskName1[a], maskOp1[a], maskMode1[b], nextByte());
                    }
                case 2:
                    if(maskRaw2[b] == 2) {
                        return Decoder::Instruct(maskName2[a], maskOp2[a], maskMode2[b], next2Bytes());
                    } else if(maskRaw2[b] == 1){
                        if((maskOp2[a] == STX || maskOp2[a] == LDX) && maskMode2[b] == ZEROX)
                            return Decoder::Instruct(maskName2[a], maskOp2[a], ZEROY, nextByte());

                        if(maskOp2[a] == LDX && maskMode2[b] == ABSX)
                            return Decoder::Instruct(maskName2[a], maskOp2[a], ABSY, nextByte());

                        return Decoder::Instruct(maskName2[a], maskOp2[a], maskMode2[b], nextByte());
                    }
                    return Decoder::Instruct(maskName2[a], maskOp2[a], maskMode2[b], zero);
                case 0:
                    if(maskRaw0[b] == 2) {
                        if(a == 3) {
                            return Decoder::Instruct(maskName0[a], maskOp0[a], IND_ABS, next2Bytes());
                        }
                        return Decoder::Instruct(maskName0[a], maskOp0[a], maskMode0[b], next2Bytes());
                    } else if(maskRaw0[b] == 1){
                        return Decoder::Instruct(maskName0[a], maskOp0[a], maskMode0[b], nextByte());
                    }
                    return Decoder::Instruct(maskName0[a], maskOp0[a], maskMode0[b], zero);
                default:
                    LOG(binify(raw.uw));
                    throw "Not a valid Decoder::Instruction";
            }
        }
    }
    return Decoder::Instruct("NOP", NOP, VOID, zero);
}

bool Decoder::hasNext() {
    if(mem->load(reg->PC, ABS).uw == 0x60 && mem->stackEmpty()) // RTS and stack is empty
        return false;

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
