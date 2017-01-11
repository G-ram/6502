#include "snoop.h"
#include "headers/log.h"

void Snoop::exec(Mem *mem) {
    for(unsigned short i = lo.udw; i <= hi.udw; i++) {
        word addr;
        addr.udw = i;
        GLOG(hexify(i) << " : " << hexify(mem->load(addr, ABS).uw));
    }
}
