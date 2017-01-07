#ifndef SNOOP_H
#define SNOOP_H

#include "peripheral.h"

class Snoop : public Peripheral {
public:
    Snoop(word alo, word ahi) {
        hi = ahi;
        lo = alo;
    };
    void exec(Mem *);
};

#endif
