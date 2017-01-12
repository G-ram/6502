#ifndef SNOOP_H
#define SNOOP_H

#include "headers/peripheral.h"

class Snoop : public Peripheral {
public:
    Snoop(word _lo, word _hi) {
        hi = _hi;
        lo = _lo;
    };
    void exec(Mem *);
    bool inRange(word) { return false; };
    bool doesDirty() { return false; }
    word map(word, word) {
        word tmp;
        return tmp;
    };
};

#endif
