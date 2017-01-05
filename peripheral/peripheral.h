#ifndef PERIPHERAL_H
#define PERIPHERAL_H

#include "../types.h"

class Mem;

class Peripheral {
public:
    Peripheral() {};
    virtual void exec(Mem &) {};
    virtual bool inRange(word) { return false; };
};

#endif
