#ifndef REG_H
#define REG_H

#include "types.h"

#define GET_BIT(w, p) (w & (1 << p)) > 0

enum StatusBit{C, Z, I, D, B, V, N};

struct Reg {
    word A;
    word X;
    word Y;
    word P;
    word S;
    word PC;
    void setStatus(StatusBit bit) {
        word mask;
        mask.uw = 1 << bit;
        P.uw |= mask.uw;
    };
    void unsetStatus(StatusBit bit) {
        word mask;
        mask.uw = ~(1 << bit);
        P.uw &= mask.uw;
    }
    bool getStatus(StatusBit bit) {
        word mask;
        mask.uw = 1 << bit;
        return (P.uw & mask.uw) > 0;
    }
};

std::ostream& operator<<(std::ostream& out, const Reg& r);
#endif
