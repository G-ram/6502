#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

// Defaults to a signed version
union word {
    signed char w;
    unsigned char uw;
    short dw;
    unsigned short udw;
    struct{signed char lo; signed char hi;} part;
    struct{unsigned char lo; unsigned char hi;} upart;
    word() { memset( this, 0, sizeof(word)); }
};

#endif
