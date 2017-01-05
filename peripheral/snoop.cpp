#include "snoop.h"
#include "../log.h"

void Snoop::exec(Mem *mem) {
    for(unsigned short i = lo.udw; i <= hi.udw; i++) {
        word addr;
        addr.udw = i;
        LOG(hexify(i) << " : " << hexify(mem->load(addr, ABS).uw));
    }
}
