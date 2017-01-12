#ifndef CPU_C_H
#define CPU_C_H

#ifdef __cplusplus
extern "C" {
#endif

	typedef void* CPU_p;
	CPU_p getCPU(unsigned short, unsigned short);
	void freeCPU(CPU_p);
	void addReader(CPU_p _cpu, unsigned short _lo, unsigned short _hi);
	void addWriter(CPU_p _cpu, unsigned short _lo, unsigned short _hi);
    void step(CPU_p);
    unsigned long long getCycles(CPU_p);

#ifdef __cplusplus
}
#endif

#endif
