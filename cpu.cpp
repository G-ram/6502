#include <iostream>

#include "loader.h"
#include "decoder.h"
#include "mem.h"

#include "log.h"

int main(int ac, char *av[]) {
    if(ac != 2) {
        std::cout << "usage: ./6502 <disk image>" << std::endl;
        return 0;
    }
    Loader loader = Loader(av[1]); // Creates and loads into memory the program
    Mem mem = loader.getMem();
    Reg reg = loader.getReg();
    Decoder decoder = Decoder(mem, reg);
    while(decoder.hasNext()) {
        try {
            Decoder::Instruct cur = decoder.decode(); // Seg fault
            LOG("PC: " << hexify(reg.PC.udw) << " : " << cur);
            cur.op(cur.addr, cur.mode, mem, reg);
        } catch(char const * ex){
            LOG("ERROR: " << ex);
            return 0;
        } catch(Mode ex) {
            LOG("ERROR: " << modeName[ex]);
            return 0;
        }
    };
}
