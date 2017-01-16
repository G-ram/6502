#include "headers/cpu.h"
#include "headers/log.h"
#include "bridge.h"
#include "reader.h"
#include "writer.h"

CPU_p getCPU(unsigned short _PC, unsigned short _SP,
            unsigned short _stackBase, unsigned short _interruptVector,
            unsigned short _nmiVector) {
    word PC, SP, stackBase, interruptVector, nmiVector;
    PC.udw = _PC;
    SP.udw = _SP;
    interruptVector.udw = _interruptVector;
    nmiVector.udw = _nmiVector;
    stackBase.udw = _stackBase;
    return (void *)(new CPU(PC, SP, stackBase, interruptVector, nmiVector));
}

void freeCPU(CPU_p _cpu) {
    CPU *cpu = (CPU* )_cpu;
    delete cpu;
}

void addReader(CPU_p _cpu, unsigned short _lo, unsigned short _hi, reader_callback callback) {
    CPU *cpu = (CPU* )_cpu;
    word lo, hi;
    lo.udw = _lo;
    hi.udw = _hi;
    Reader *reader = new Reader(lo, hi, callback); // Not freed at the moment
    cpu->mem->connect(reader);
}

void addWriter(CPU_p _cpu, unsigned short _lo, unsigned short _hi, writer_callback callback) {
    CPU *cpu = (CPU* )_cpu;
    word lo, hi;
    lo.udw = _lo;
    hi.udw = _hi;
    Writer *writer = new Writer(lo, hi, callback); // Not freed at the moment
    cpu->mem->connect(writer);
}

void NMI(CPU_p _cpu) {
    CPU *cpu = (CPU* )_cpu;
    cpu->NMI();
}

unsigned char read(CPU_p _cpu, unsigned short _addr) {
    CPU *cpu = (CPU* )_cpu;
    word addr;
    addr.udw = _addr;
    return cpu->mem->load(addr, ABS).uw;
}

void step(CPU_p _cpu) {
    CPU *cpu = (CPU* )_cpu;
    cpu->step();
}

unsigned long long getCycles(CPU_p _cpu) {
    CPU *cpu = (CPU* )_cpu;
    return cpu->getCycles();
}
