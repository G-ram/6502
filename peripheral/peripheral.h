#ifndef PERIPHERAL_H
#define PERIPHERAL_H

#include "../types.h"

class Mem;

class Peripheral {
protected:
    word hi;
    word lo;
public:
    Peripheral() {};
    virtual void exec(Mem *) {};
};

#endif
