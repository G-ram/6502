#include "alu.h"
#include "loader.h"
#include "instruct.h"
#include "mem.h"

int main(uint32_t ac, char *av[]) {
    if(ac != 2) {
        std::cout << "usage: ./6502 <disk image>" << std::endl;
        return 0;
    }
    Loader loader = Loader(av[1]); // Creates and loads into memory the program
    Mem mem = loader.getMem();
    Reg reg = loader.getReg();
    Decoder decoder = decoder();
    while(decoder.hasNext()) {
        Instruct cur = decoder.decode(mem, reg);
        cur.op(cur.addr, cur.mode, mem, reg);
    };
}
