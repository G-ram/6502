#ifndef PERIPHERAL_H
#define PERIPHERAL_H

#include "types.h"
#include "mem.h"

class Peripheral {
public:
    Peripheral() {};
    virtual void exec(Mem &);
    virtual bool inRange(word addr);
};
#endif
