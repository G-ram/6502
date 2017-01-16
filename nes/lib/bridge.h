#ifndef CPU_C_H
#define CPU_C_H

#ifdef __cplusplus
extern "C" {
#endif

	typedef void* CPU_p;
	typedef unsigned char (*reader_callback) (unsigned short);
	typedef void (*writer_callback) (unsigned short, unsigned char);
	CPU_p getCPU(unsigned short, unsigned short, unsigned short, unsigned short, unsigned short);
	void freeCPU(CPU_p);
	void addReader(CPU_p _cpu, unsigned short _lo, unsigned short _hi, reader_callback);
	void addWriter(CPU_p _cpu, unsigned short _lo, unsigned short _hi, writer_callback);
	void NMI(CPU_p _cpu);
	unsigned char read(CPU_p _cpu, unsigned short);
    void step(CPU_p);
    unsigned long long getCycles(CPU_p);

#ifdef __cplusplus
}
#endif

#endif
