#include <string>

#include "bridge.h"
#include "headers/cpu.h"

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

unsigned char read(CPU_p _cpu, unsigned short _addr, unsigned char _mode) {
    CPU *cpu = (CPU* )_cpu;
    word addr;
    addr.udw = _addr;
    Mode mode = static_cast<Mode>(_mode);
    return cpu->mem->load(addr, mode).uw;
}

void write(CPU_p _cpu, unsigned char _data, unsigned short _addr, unsigned char _mode) {
    CPU *cpu = (CPU* )_cpu;
    word data, addr;
    data.upart.lo = _data;
    addr.udw = _addr;
    Mode mode = static_cast<Mode>(_mode);
    cpu->mem->store(data, addr, mode);
}

void copyTo(CPU_p _cpu, char *_data, size_t length, unsigned short _addr) {
    CPU *cpu = (CPU* )_cpu;
    word addr;
    addr.udw = _addr;
    std::string data(_data, _data + length);
    cpu->mem->copyTo(data, addr);
}

void step(CPU_p _cpu) {
    CPU *cpu = (CPU* )_cpu;
    cpu->step();
}

unsigned long long getCycles(CPU_p _cpu) {
    CPU *cpu = (CPU* )_cpu;
    return cpu->getCycles();
}
