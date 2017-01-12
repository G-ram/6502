#ifndef READER_H
#define READER_H
#include "headers/peripheral.h"

extern unsigned short readCallBack(unsigned short);

class Reader : public Peripheral {
public:
    Reader(word _lo, word _hi) {
        hi = _hi;
        lo = _lo;
    };
    void exec(Mem *) {};
    bool inRange(word addr) {
        if(addr.udw >= lo.udw && addr.udw <= hi.udw)
            return true;
        return false;
    };
    bool doesDirty() { return false; }
    word map(word, word);
};

#endif
