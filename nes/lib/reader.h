#ifndef READER_H
#define READER_H
#include "headers/peripheral.h"

typedef unsigned char (*reader_callback) (unsigned short);

class Reader : public Peripheral {
private:
    reader_callback callback;
public:
    Reader(word _lo, word _hi, reader_callback callback) :
        callback(callback) {
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
