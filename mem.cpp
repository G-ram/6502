#include <string>
#include "mem.h"

// Assumes Little Endian Processor
word Mem::translate(word addr, Mode mode, Reg &reg) {
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
            tmp.udw = addr.udw + reg.X.udw;
            break;
        case ABSY:
            tmp.udw = addr.udw + reg.Y.udw;
            break;
        case ZEROX:
            tmp.udw = reg.X.udw;
            break;
        case ZEROY:
            tmp.udw = reg.Y.udw;
            break;
        case IDX_IND:
            word a;
            a.uw = reg.X.uw + addr.uw;
            tmp.upart.lo = load(a, ABS).uw;
            a.uw++;
            tmp.upart.hi = load(a, ABS).uw;
            break;
        case IND_IDX:
            word a;
            a.upart.lo = load(addr.uw, ABS).uw;
            addr.uw++;
            a.upart.hi = load(addr.uw, ABS).uw;
            tmp.dw = a.udw + reg.Y.udw;
            break;
        case REL:
            if(addr.w < 0) {
                word a;
                a.w = ~addr.w;
                a.uw++;
                tmp.dw = reg.PC.udw - a.uw; // 2's complement
            } else {
                tmp.dw = reg.PC.udw + addr.w;
            }
        default:
            throw "Does not need to be translated";
    }
    return tmp;
}

word Mem::load(word addr, Mode mode) {
    word tmp;
    try {
        word taddr = translate(addr, Mode mode);
        tmp.uw = data[taddr];
    } catch(std::string ex){
        switch(mode){
            case IMM:
                tmp.uw = addr.uw;
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
        word taddr = translate(addr, Mode mode);
        data[taddr] = newData.uw;
        findPeripheral(addr);
    } catch(std::string ex){
        return;
    }
}

void Mem::push(word newData) {
    stack.push_back(word);
}

word Mem::pop() {
    if(stack.empty())
        throw "stack not balanced";

    return stack.pop_back();
}

void Mem::copyTo(std::string raw, word addr) {
    uint16_t taddr;
    memcpy(&taddr, addr, sizeof(word));
    memcpy(&data[taddr], raw.c_str(), sizeof(char) * raw.length());
}

void Mem::findPeripheral(word addr) {
    for(auto p : peripherals) {
        if(p.inRange(addr)) {
            p.exec(*this);
        }
    }
}
