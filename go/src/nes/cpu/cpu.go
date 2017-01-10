package cpu

/*
#cgo LDFLAGS: -l6502
#include <stdlib.h>
#include "bridge.h"
*/
import "C"

type CPU struct {
	p C.CPU_p
}

func getCPU(uint16 PC, uint16 SP) CPU {
	var cpu CPU
	cpu.p = C.getCPU(C.ushort(PC), C.ushort(SP))
	return ret
}

func (cpu CPU) freeCPU() {
	C.freeCPU(cpu.p)
}

func (cpu CPU) read(uint16 addr, byte mode) {
	C.read(cpu.p, C.ushort(addr), C.uchar(mode))
}

func (cpu CPU) write(byte data, uint16 addr, byte mode) {
	C.write(cpu.p, C.uchar(data), C.ushort(addr), C.uchar(mode))
}

func (cpu CPU) copyTo(string _data, uint16 addr) {
	data := C.CString(_data)
	C.copyTo(cpu.p, data, len(_data), C.ushort(addr))
	C.free(data)
}

func (cpu CPU) step() {
	C.step(cpu.p)
}

func (cpu CPU) getCycles() uint64 {
	return uint64(C.getCycles())
}
