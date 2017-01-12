#include "headers/cpu.h"
#include "headers/log.h"

CPU::CPU(word PC, word SP) : cycles(0) {
    reg->PC = PC;
    reg->S = SP;
    mem = new Mem(reg);
    decoder = new Decoder(mem, reg);
}

void CPU::step() {
    try {
        Decoder::Instruct cur = decoder->decode();
        LOG("PC: " << hexify(reg->PC.udw) << " : " << cur);
        cur.op(cur.addr, cur.mode, mem, reg);
        cycles += cur.cycles;
        cycles += mem->cycles;
        mem->cycles = 0; // Reset page cycles
    } catch(char const * ex){
        LOG("ERROR: " << ex);
    } catch(Mode ex) {
        LOG("ERROR: " << modeName[ex]);
    }
}

CPU::~CPU() {
    delete decoder;
    delete mem;
}
