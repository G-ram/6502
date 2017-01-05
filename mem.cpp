#include <string>
#include "mem.h"
#include "log.h"
#include "peripheral/peripheral.h"

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
            tmp.upart.lo = load(addr, ABS).uw;
            addr.udw++;
            tmp.upart.hi = load(addr, ABS).uw;
            break;
        case ABSX:
            tmp.udw = addr.udw + reg->X.udw;
            break;
        case ABSY:
            tmp.udw = addr.udw + reg->Y.udw;
            break;
        case ZEROX:
            tmp.udw = reg->X.udw;
            break;
        case ZEROY:
            tmp.udw = reg->Y.udw;
            break;
        case IDX_IND:
        {
            word a;
            a.uw = reg->X.uw + addr.uw;
            tmp.upart.lo = load(a, ABS).uw;
            a.uw++;
            tmp.upart.hi = load(a, ABS).uw;
            break;
        }
        case IND_IDX:
        {
            word a;
            addr.upart.hi = 0;
            a.upart.lo = load(addr, ABS).uw;
            addr.uw++;
            a.upart.hi = load(addr, ABS).uw;
            tmp.dw = a.udw + reg->Y.udw;
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
    } catch(char const *ex){
        return;
    }
}

void Mem::push(word newData) {
    stack.push_back(newData);
}

word Mem::pop() {
    if(stack.empty())
        throw "stack not balanced";

    word tmp = stack.back();
    stack.pop_back();
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
