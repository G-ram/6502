#ifndef WRITER_H
#define WRITER_H
#include "headers/peripheral.h"

typedef void (*writer_callback) (unsigned short, unsigned char);

class Writer : public Peripheral {
private:
    writer_callback callback;
public:
    Writer(word _lo, word _hi, writer_callback callback) :
        callback(callback) {
        hi = _hi;
        lo = _lo;
    };
    virtual void exec(Mem *) {};
    virtual bool inRange(word addr) {
        if(addr.udw >= lo.udw && addr.udw <= hi.udw)
            return true;
        return false;
    };
    virtual bool doesDirty() { return true; };
    virtual word map(word, word);
};

#endif
