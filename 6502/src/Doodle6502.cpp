#include "Doodle6502.h"

Doodle6502::Doodle6502()
{
	memory.fill(0x00);

	//test program

	writeMemory(0xfffd, 0x08);
	writeMemory(0x0004, 0x44);
	writeMemory(0x0800, 0x06);
	writeMemory(0x0801, 0x04);
	writeMemory(0x0802, 0x09);
	writeMemory(0x0803, 0x33);

	//

	reset();
}

Doodle6502::~Doodle6502()
{
}

void Doodle6502::reset()
{
	//reset the CPU
	A = 0;
	X = 0;
	Y = 0;
	sp = 0xFD;
	p = 0x34;
	pc = (readMemory(0xFFFD) << 8) | readMemory(0xFFFC); // Fetch reset vector
	
}

void Doodle6502::executeInstruction()
{
	uint8_t opcode = readMemory(pc++);
	Instruction& instruction = opcodeTable[opcode];
	(this->*instruction.execute)(getAddress(instruction.addressingmode));
}

uint8_t Doodle6502::readMemory(uint16_t address)
{
	return memory[address];
}

void Doodle6502::writeMemory(uint16_t address, uint8_t value)
{
	memory[address] = value;
}

void Doodle6502::setFlag(StatusFlag flag, bool value)
{
	if (value)
	{
		p |= (1 << flag);
	}
	else {
		p &= ~(1 << flag);
	}
}

bool Doodle6502::getFlag(StatusFlag flag)
{
	return (p & (1 << flag)) != 0;
}

void Doodle6502::pushStack(uint8_t value)
{
	writeMemory(0x100 + sp, value);
	sp--;
}

uint8_t Doodle6502::popStack()
{
	sp++;
	return readMemory(0x100 + sp);
}

uint16_t Doodle6502::getAddress(AddressingMode mode)
{
	uint16_t lo;
	uint16_t hi;
	uint16_t eaddr;
	switch (mode) {
	case accumulator:
		return 0;
	case implicit:
		return 0;
	case indexed_indirect:
		lo = readMemory(pc++) + X;
		hi = lo + 1;
		eaddr = readMemory(lo) + (readMemory(hi) << 8);
		return eaddr;
	case zeropage:
		return readMemory(pc++);
	case immediate:
		return pc++;

	}
}



//Many of these functions for the opcodes are either taken from or inspired by Gianlucag's 6502 emulator.

void Doodle6502::BRK(uint16_t address)
{
	pc++;
	pushStack((pc >> 8));
	pushStack(pc);
	pushStack(p | 0x20 | 0x10);
	setFlag(I, 1);
	pc = ((readMemory(0xFFFE) << 8) + (readMemory(0xFFFF)));
	return;
}

void Doodle6502::ORA(uint16_t address)
{
	uint8_t o = A | readMemory(address);
	setFlag(N, o & 0x80);
	setFlag(Z, !o);
	A = o;
	return;
}

void Doodle6502::ILLEGAL(uint16_t address)
{
	return;
}

void Doodle6502::ASL(uint16_t address)
{
	uint8_t m = readMemory(address);
	setFlag(C, m & 0x80);
	m <<= 1;
	m &= 0xff;
	setFlag(N, m & 0x80);
	setFlag(Z, !m);
	writeMemory(address, m);
	return;
}

void Doodle6502::PHP(uint16_t)
{
	uint8_t tmp = p;
	setFlag(I, 1);
	setFlag(D, 1);
	pushStack(p);
	p = tmp;

}

