#ifndef CPU_C_H
#define CPU_C_H

#ifdef __cplusplus
extern "C" {
#endif

	typedef void* CPU_p;
	CPU_p getCPU(unsigned short, unsigned short);
	void freeCPU(CPU_p);
	unsigned char read(CPU_p, unsigned short, unsigned char);
    void write(CPU_p, unsigned char, unsigned short, unsigned char);
    void copyTo(CPU_p, char *, size_t, unsigned short);
    void step(CPU_p);
    unsigned int getCycles(CPU_p);

#ifdef __cplusplus
}
#endif

#endif
