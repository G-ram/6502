#include <string>

#include "cpu_c.h"
#include "cpu.h"

CPU getCPU(unsigned short _PC, unsigned short _SP) {
    word PC, SP;
    PC.udw = _PC;
    SP.udw = _SP;
    return new CPU(PC, SP);
}

void freeCPU(CPU _cpu) {
    CPU *cpu = (CPU* )_cpu;
    delete cpu;
}

unsigned char read(CPU _cpu, unsigned short _addr, unsigned char _mode) {
    CPU *cpu = (CPU* )_cpu;
    word addr;
    addr.udw = _addr;
    Mode mode = static_cast<Mode>(_mode);
    return cpu->mem->read(addr, mode).uw;
}

void write(CPU _cpu, unsigned char _data, unsigned short _addr, unsigned char _mode) {
    CPU *cpu = (CPU* )_cpu;
    word data, addr;
    data.upart.lo = _data;
    addr.udw = _addr;
    Mode mode = static_cast<Mode>(_mode);
    cpu->mem->read(data, mode);
}

void copyTo(CPU _cpu, char *_data, size_t length, unsigned short _addr) {
    CPU *cpu = (CPU* )_cpu;
    word addr;
    addr.udw = _addr;
    std::string data;
    data.reserve(length);
    memcpy(data.c_str(), _data, sizeof(char) * length);
    cpu->mem->copyTo(data, addr);
}

void step(CPU _cpu) {
    CPU *cpu = (CPU* )_cpu;
    cpu->step();
}

size_t getCycles(CPU _cpu) {
    CPU *cpu = (CPU* )_cpu;
    return cpu->getCycles();
}
