#include "writer.h"

word Writer::map(word addr, word data) {
    word tmp;
    callback(addr.udw, data.uw);
    return tmp;
}
