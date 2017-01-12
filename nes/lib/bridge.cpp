#include "headers/cpu.h"
#include "headers/log.h"
#include "bridge.h"
#include "reader.h"
#include "writer.h"

CPU_p getCPU(unsigned short _PC, unsigned short _SP) {
    word PC, SP;
    PC.udw = _PC;
    SP.udw = _SP;
    return (void *)(new CPU(PC, SP));
}

void freeCPU(CPU_p _cpu) {
    CPU *cpu = (CPU* )_cpu;
    delete cpu;
}

void addReader(CPU_p _cpu, unsigned short _lo, unsigned short _hi, reader_callback callback) {
    LOG("READER");
    CPU *cpu = (CPU* )_cpu;
    word lo, hi;
    lo.udw = _lo;
    hi.udw = _hi;
    Reader *reader = new Reader(lo, hi, callback); // Not freed at the moment
    cpu->mem->connect(reader);
}

void addWriter(CPU_p _cpu, unsigned short _lo, unsigned short _hi, writer_callback callback) {
    LOG("WRITER");
    CPU *cpu = (CPU* )_cpu;
    word lo, hi;
    lo.udw = _lo;
    hi.udw = _hi;
    Writer *writer = new Writer(lo, hi, callback); // Not freed at the moment
    cpu->mem->connect(writer);
}

void step(CPU_p _cpu) {
    LOG("STEP");
    CPU *cpu = (CPU* )_cpu;
    cpu->step();
}

unsigned long long getCycles(CPU_p _cpu) {
    CPU *cpu = (CPU* )_cpu;
    return cpu->getCycles();
}
