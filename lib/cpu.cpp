#include "headers/cpu.h"
#include "headers/log.h"

bool print;

CPU::CPU(word PC, word SP, word stackBase, word _interruptVector, word _nmiVector)
    : cycles(0) {
    interruptVector = _interruptVector;
    nmiVector = _nmiVector;
    reg = new Reg();
    reg->setStatus(I); // Also begins high for some reason
    reg->setStatus(unused); // For some reason this is always high
    reg->PC = PC;
    reg->S = SP;
    mem = new Mem(reg, stackBase);
    decoder = new Decoder(mem, reg);
    print = false;
}

void CPU::step() {
    try {
        // word tmpPC = reg->PC;
        Decoder::Instruct cur = decoder->decode();
        // GLOG("PC: " << hexify(tmpPC.udw) << " SP: " << hexify(reg->S.uw) << " : " << cur << " Cycles: " << cycles);
        // GNLOG("PC=" << hexify(tmpPC.udw) << " A=" << hexify(reg->A.uw) <<
        //     " X=" << hexify(reg->X.uw) << " Y=" << hexify(reg->Y.uw) << " P=" << hexify(reg->P.uw) <<
        //     " SP=" << hexify(reg->S.uw) << ", ins=" << 0 << ", C=" << reg->getStatus(C) <<
        //     " Z=" << reg->getStatus(Z) << " I=" << reg->getStatus(I) <<  " D=" << reg->getStatus(D) <<
        //     " B=" << reg->getStatus(B) << " O=" << reg->getStatus(V) <<  " S=" << reg->getStatus(N) <<
        //     " OP=" << cur.name);
        // word addr, tmp;
        // addr.udw = 0x87FE;
        // tmp = mem->load(addr, ABS);

        // if(tmp.uw != 0) {
            // print = true;
        // }
        if(print == true) {
            // GLOG("PC=" << hexify(tmpPC.udw) << " A=" << hexify(reg->A.uw) <<
            //     " X=" << hexify(reg->X.uw) << " Y=" << hexify(reg->Y.uw) << " P=" << hexify(reg->P.uw) <<
            //     " SP=" << hexify(reg->S.uw) << ", ins=" << 0 << ", C=" << reg->getStatus(C) <<
            //     " Z=" << reg->getStatus(Z) << " I=" << reg->getStatus(I) <<  " D=" << reg->getStatus(D) <<
            //     " B=" << reg->getStatus(B) << " O=" << reg->getStatus(V) <<  " S=" << reg->getStatus(N) <<
            //     " OP= " << cur.name);
            // GLOG("PC=" << hexify(tmpPC.udw) << " A=" << hexify(reg->A.uw) <<
            //     " X=" << hexify(reg->X.uw) << " Y=" << hexify(reg->Y.uw) << " P=" << hexify(reg->P.uw) <<
            //     " SP=" << hexify(reg->S.uw) << ", ins=" << 0 << ", C=" << reg->getStatus(C) <<
            //     " Z=" << reg->getStatus(Z) << " I=" << reg->getStatus(I) <<  " D=" << reg->getStatus(D) <<
            //     " B=" << reg->getStatus(B) << " O=" << reg->getStatus(V) <<  " S=" << reg->getStatus(N) <<
            //     " OP= " << cur << " Location: " << hexify(tmp.uw));
            // GLOG("PC=" << hexify(tmpPC.udw) << " A=" << hexify(reg->A.uw) <<
            //     " X=" << hexify(reg->X.uw) << " Y=" << hexify(reg->Y.uw) << " P=" << hexify(reg->P.uw) <<
            //     " SP=" << hexify(reg->S.uw) << ", ins=" << 0 << ", C=" << reg->getStatus(C) <<
            //     " Z=" << reg->getStatus(Z) << " I=" << reg->getStatus(I) <<  " D=" << reg->getStatus(D) <<
            //     " B=" << reg->getStatus(B) << " O=" << reg->getStatus(V) <<  " S=" << reg->getStatus(N) <<
            //     " OP=" << cur);
        }
        // GLOG("PC=" << hexify(tmpPC.udw) << " A=" << hexify(reg->A.uw) <<
        //     " X=" << hexify(reg->X.uw) << " Y=" << hexify(reg->Y.uw) << " P=" << hexify(reg->P.uw) <<
        //     " SP=" << hexify(reg->S.uw) << ", ins=" << 0 << ", C=" << reg->getStatus(C) <<
        //     " Z=" << reg->getStatus(Z) << " I=" << reg->getStatus(I) <<  " D=" << reg->getStatus(D) <<
        //     " B=" << reg->getStatus(B) << " O=" << reg->getStatus(V) <<  " S=" << reg->getStatus(N) <<
        //     " OP=" << cur << " Location: " << hexify(tmp.udw));
        // GNLOG("PC=" << hexify(tmpPC.udw) << " A=" << hexify(reg->A.uw) <<
        //     " X=" << hexify(reg->X.uw) << " Y=" << hexify(reg->Y.uw) << " P=" << hexify(reg->P.uw) <<
        //     " SP=" << hexify(reg->S.uw) << ", ins=" << 0 << ", C=" << reg->getStatus(C) <<
        //     " Z=" << reg->getStatus(Z) << " I=" << reg->getStatus(I) <<  " D=" << reg->getStatus(D) <<
        //     " B=" << reg->getStatus(B) << " O=" << reg->getStatus(V) <<  " S=" << reg->getStatus(N) <<
        //     " OP=" << cur);
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

void CPU::NMI() {
    word tmp = nmiVector;
    mem->push16(reg->PC);
    mem->push8(reg->P);
    reg->PC.upart.lo = mem->load(tmp, ABS).uw;
    tmp.udw += 1;
    reg->PC.upart.hi = mem->load(tmp, ABS).uw;
    reg->setStatus(I);
}

CPU::~CPU() {
    delete reg;
    delete decoder;
    delete mem;
}
