#ifndef PERIPHERAL_H
#define PERIPHERAL_H

#include "../types.h"
#include "../mem.h"

class Peripheral {
protected:
    word hi;
    word lo;
public:
    Peripheral() {};
    virtual void exec(Mem *) {};
};

#endif
