#include <string>
#include "headers/mem.h"
#include "headers/peripheral.h"
#include "headers/log.h"

extern bool print;

std::string modeName[13] = {"IMM", "ABS", "ZERO", "IND_ABS", "ABSX",
                            "ABSY", "ZEROX", "ZEROY", "IDX_IND",
                            "IND_IDX", "REL", "ACC", "VOID"};
// Assumes Little Endian Processor
word Mem::translate(word addr, Mode mode) {
    word tmp;
    switch(mode){
        case ABS:
            tmp.udw = addr.udw;
            break;
        case ZERO:
            tmp.uw = addr.uw;
            break;
        case IND_ABS:
            tmp = read16WithPageBoundaryBug(addr);
            break;
        case ABSX:
            tmp.udw = addr.udw + reg->X.uw;
            if(!pagesEqual(tmp, addr)) // Page boundary
                cycles++;
            break;
        case ABSY:
            tmp.udw = addr.udw + reg->Y.uw;
            if(!pagesEqual(tmp, addr)){ // Page boundary
                cycles++;
            }
            break;
        case ZEROX:
            tmp.udw = addr.uw + reg->X.uw;
            break;
        case ZEROY:
            tmp.udw = addr.uw + reg->Y.uw;
            break;
        case IDX_IND:
        {
            word a;
            a.uw = reg->X.uw + addr.uw;
            tmp = read16WithPageBoundaryBug(a);
            break;
        }
        case IND_IDX:
        {
            word a;
            addr.upart.hi = 0;
            a = read16WithPageBoundaryBug(addr);
            tmp.dw = a.udw + reg->Y.uw;
            if(!pagesEqual(tmp, a)) // Page boundary
                cycles++;
            break;
        }
        case REL:
            if(addr.w < 0) {
                word a;
                a.w = ~addr.w;
                a.uw++;
                tmp.udw = reg->PC.udw - a.uw; // 2's complement
            } else {
                tmp.dw = reg->PC.udw + addr.w;
            }
            break;
        default:
            throw "Does not need to be translated";
    }
    return tmp;
}

word Mem::load(word addr, Mode mode) {
    word tmp;
    try {
        word taddr = translate(addr, mode);
        tmp.uw = data[taddr.udw];
        word zero;
        for(auto p : peripherals) {
            if(p->inRange(taddr) && !(p->doesDirty())) { // Makes no assumption on consistency/coherency
                tmp = p->map(taddr, zero);
            }
        }
        if(tmp.uw == 0x01 && taddr.udw == 0x4016) { // Remove, for debugging
            print = true;
        }
    } catch(char const* ex){
        switch(mode){
            case IMM:
                tmp.uw = addr.uw;
                break;
            case ACC:
                throw ACC;
            default: //VOID and IMPLIED
                throw VOID;
        }
    }
    return tmp;
}

void Mem::store(word newData, word addr, Mode mode) {
    try {
        word taddr = translate(addr, mode);
        data[taddr.udw] = newData.uw;
        for(auto p : peripherals) {
            if(p->inRange(taddr) && p->doesDirty()) // Makes no assumption on consistency/coherency
                p->map(taddr, newData);
        }
    } catch(char const *ex){
        return;
    }
}

void Mem::push8(word newData) {
    word addr;
    addr.udw = stackBase.udw + reg->S.udw;
    store(newData, addr, ABS);
    reg->S.udw -= 1;
}

word Mem::pop8() {
    word addr;
    addr.udw = stackBase.udw + reg->S.udw + 1;
    word tmp = load(addr, ABS);
    reg->S.udw += 1;
    return tmp;
}

void Mem::push16(word newData) {
    word addr;
    word hi, lo;
    lo.udw = newData.udw & 0xff;
    hi.udw = newData.udw >> 8;
    addr.udw = stackBase.udw + reg->S.udw - 1;
    store(lo, addr, ABS);
    addr.udw += 1;
    store(hi, addr, ABS);
    reg->S.uw -= 2;
}

word Mem::pop16() {
    word addr;
    addr.udw = stackBase.udw + reg->S.udw + 1;
    word tmp = load(addr, ABS);
    tmp.upart.lo = load(addr, ABS).uw;
    addr.udw += 1;
    tmp.upart.hi = load(addr, ABS).uw;
    reg->S.uw += 2;
    return tmp;
}

void Mem::copyTo(std::string raw, word addr) {
    unsigned short taddr;
    memcpy(&taddr, &addr, sizeof(word));
    memcpy(&data[taddr], raw.c_str(), sizeof(char) * raw.length());
}

void Mem::broadcast() {
    for(auto p : peripherals) {
        p->exec(this);
    }
}

std::string Mem::dump() {
    return std::string(reinterpret_cast<char *>(data), MEM_SIZE);
}

bool Mem::pagesEqual(word addrA, word addrB) {
    return (addrA.udw & 0xFF00) == (addrB.udw & 0xFF00);
}

word Mem::read16WithPageBoundaryBug(word addr) {
    word lo, hi, tmp;
    lo.udw = addr.udw;
    if((addr.udw & 0xff) == 0xff) {
        hi.udw = addr.udw & 0xff00;
    } else {
        hi.udw = addr.udw + 1;
    }
	tmp.udw = load(lo, ABS).udw | load(hi, ABS).udw << 8;
	return tmp;
}
