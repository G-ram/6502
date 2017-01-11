#ifndef SNOOP_H
#define SNOOP_H

#include "headers/peripheral.h"

class Snoop : public Peripheral {
public:
    Snoop(word alo, word ahi) {
        hi = ahi;
        lo = alo;
    };
    void exec(Mem *);
    bool inRange(word) { return false; };
    word map(word, word) {
        word tmp;
        return tmp;
    };
};

#endif
