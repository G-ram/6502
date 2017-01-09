#ifndef CPU_C_H
#define CPU_C_H

#ifdef __cplusplus
extern "C" {
#endif

	typedef void* CPU;
	CPU getCPU(unsigned short, unsigned short);
	void freeCPU(CPU);
	void read(CPU, unsigned short, unsigned char);
    void write(CPU, unsigned char, unsigned short, unsigned char);
    void copyTo(CPU, char *, size_t, unsigned short);
    void step(CPU);
    size_t getCycles(CPU);

#ifdef __cplusplus
}
#endif

#endif
