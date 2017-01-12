#include "reader.h"

word Reader::map(word addr, word data) {
    word tmp;
    tmp.udw = callback(addr.udw);
    return tmp;
}
