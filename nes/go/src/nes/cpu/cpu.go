package cpu

/*
#cgo LDFLAGS: -lc++ -L. -l6502nes -l6502
#include <stdlib.h>
#include "/Users/Graham/Desktop/6502/nes/go/src/nes/cpu/bridge.h"
*/
import "C"

type CPU struct {
	Console *Console
	p C.CPU_p
}

func getCPU(console *Console) CPU {
	c := &CPU{Console: console}
	PC := c.read16(ResetVector)
	SP := 0xFD
	c.p = C.getCPU(C.ushort(PC), C.ushort(SP))
	var lo, hi uint16
	lo = 0x2000
	hi = 0xFFFF
	c.addReader(lo, hi)
	c.addWriter(lo, hi)
	return c
}

func (c *CPU) freeCPU() {
	C.freeCPU(cpu.p)
}

func (c *CPU) addReader(uint16 lo, uint16 hi) {
	C.addReader(c.p, C.ushort(lo), C.ushort(hi))
}

func (c *CPU) addWriter(uint16 lo, uint16 hi) {
	C.addWriter(c.p, C.ushort(lo), C.ushort(hi))
}

func (c *CPU) step() {
	C.step(c.p)
}

func (c *CPU) getCycles() uint64 {
	return uint64(C.getCycles())
}

//export readerCallBack
func readerCallBack(addr C.ushort) C.uchar { //Won't work yet because operator on c
	var result byte

	switch {
	case address >= 0x2000 && address < 0x4000:
		switch address & 0x7 {
		case 2:
			result = c.Console.PPU.StatusRegister()
		case 4:
			result = c.Console.PPU.ReadSPR()
		case 7:
			result = c.Console.PPU.ReadData()
		default:
			log.Printf("Unknown read @ %x", address)
		}
	case address == 0x4016:
		result = c.Console.Joypads[0].Read()
	case address == 0x4017:
		result = c.Console.Joypads[1].Read()
	case address >= 0x6000 && address <= 0xFFFF:
		result = c.Console.Cart.Read(address, false)
	default:
		// log.Printf("Unimplemented CPU mem read @ %x", address)
		result = 0xFF
	}

	return C.uchar(result)
}

//export writerCallBack
func writerCallBack(addr C.ushort, data C.uchar) C.ushort { //Won't work yet because operator on c
	cycles := 0
	switch {
	case address >= 0x2000 && address < 0x4000:
		switch address & 0x7 {
		case 0x0:
			c.Console.PPU.SetControlRegister(value)
		case 0x1:
			c.Console.PPU.SetMaskRegister(value)
		case 0x3:
			c.Console.PPU.SetSPRAddress(value)
		case 0x4:
			c.Console.PPU.WriteSPR(value)
		case 0x5:
			c.Console.PPU.WriteScroll(value)
		case 0x6:
			c.Console.PPU.WriteDataAddress(value)
		case 0x7:
			c.Console.PPU.WriteData(value)
		default:
			log.Printf("Unknown write @ %x", address)
		}
	case address == 0x4016:
		c.Console.Joypads[0].Write(value)
	case address == 0x4017:
		c.Console.Joypads[1].Write(value)
	case address == 0x4014:
		c.Console.PPU.SetSPRAddress(0)
		var i uint16
		for i = 0; i < 0x100; i++ {
			sprValue := c.read(uint16(value)*0x100 + i)
			c.Console.PPU.WriteSPR(sprValue)
		}
		cycles = 512
	case address >= 0x6000 && address < 0x8000:
		c.Console.Cart.Write(address, value, false)
	case address >= 0x8000 && address <= 0xFFFF:
		c.Console.Cart.Write(address, value, false)
	default:
		// log.Printf("Unimplemented CPU mem write @ %x", address)
	}

	// cycles ADD CYCLES
}

func (c *CPU) read16(address uint16) uint16 {
	var low uint16 = address
	var high uint16 = address + 1

	return uint16(c.read(low)) | uint16(c.read(high))<<8
}
